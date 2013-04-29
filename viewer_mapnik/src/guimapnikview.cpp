#include "guimapnikview.h"
#include "ui_guimapnikview.h"

//QT
#include <QPainter>

//DM
#include "dmlogger.h"
#include "systemmapnikwrapper.h"
#include <systemmapnikwrapper.h>

//Mapnik
#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/line_symbolizer.hpp>
#include <mapnik/polygon_symbolizer.hpp>
#include <mapnik/text_symbolizer.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/graphics.hpp>
#include <mapnik/color.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/expression.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/load_map.hpp>

#include "guistyledefinition.h"


using namespace mapnik;

struct mapnik_private {
    QMap<QString,  boost::shared_ptr<SystemMapnikWrapper> > datasources_;
};

GUIMapnikView::GUIMapnikView(QWidget *parent, DM::System * sys) :
    sys_(sys),
    QWidget(parent),
    ui(new Ui::GUIMapnikView),
    d(new mapnik_private)
{
    ui->setupUi(this);

    std::string mapnik_dir("/usr/local/Cellar/mapnik/2.1.0/lib");
    DM::Logger(DM::Debug) << " looking for 'shape.input' plugin in... " << mapnik_dir << "/mapnik/input/";
    datasource_cache::instance()->register_datasources(mapnik_dir + "/mapnik/input/");
    DM::Logger(DM::Debug) << " looking for DejaVuSans font in... " << mapnik_dir << "/mapnik/fonts/DejaVuSans.ttf";
    freetype_engine::register_font(mapnik_dir + "/mapnik/fonts/DejaVuSans.ttf");

    map_ = 0;
    if (!sys)
        return;

    this->init_mapnik();
    this->drawMap();
}

GUIMapnikView::~GUIMapnikView()
{
    delete ui;
    delete map_;
    delete d;
}

void GUIMapnikView::paintEvent(QPaintEvent *ev)
{
    if(!sys_)
        return;
    QPainter painter(this);

    painter.drawPixmap(QPoint(0, 0),pix_);
    painter.end();
}

void GUIMapnikView::resizeEvent(QResizeEvent *event)
{
    if (!sys_)
        return;
    this->drawMap();
    QWidget::resizeEvent(event);
}

void GUIMapnikView::init_mapnik() {
    if (!sys_)
        return;

    if (map_)
        delete map_;


    map_ = new Map(this->width(),this->height());


    DM::Logger(DM::Debug) << this->width();
    map_->set_background(color("white"));

}


void GUIMapnikView::drawMap()
{
    if (!sys_)
        return;
    map_->set_height(this->height());
    map_->set_width(this->width());
    map_->zoom_all();
    image_32 buf(map_->width(),map_->height());
    agg_renderer<image_32> ren(*map_,buf);
    ren.apply();

    QImage image((uchar*)buf.raw_data(),map_->width(),map_->height(),QImage::Format_ARGB32);
    pix_ = QPixmap::fromImage(image.rgbSwapped());
}

void GUIMapnikView::setSystem(DM::System * sys)
{
    this->sys_ = sys;
    this->init_mapnik();
    this->drawMap();
}

void GUIMapnikView::addLayer(QString dm_layer)
{
    parameters p;
    p["type"]="dm";
    p["view_name"]= dm_layer.toStdString();
    p["view_type"]= this->sys_->getViewDefinition(dm_layer.toStdString())->getType();
    boost::shared_ptr<SystemMapnikWrapper> ds(new SystemMapnikWrapper(p, true, sys_));

    emit new_layer_added(dm_layer);

    d->datasources_[dm_layer] = ds;

    //Default symbolizer Edge
    feature_type_style edges_style;
    rule edge_rule_on;
    edge_rule_on.append(stroke(color(0, 0, 0), 1));
    edges_style.add_rule(edge_rule_on);
    map_->insert_style("default_edge",edges_style);

    emit new_style_added(dm_layer, "default_edge");

    //Default symbolizer Polygon
    feature_type_style polygon_style;
    rule polygon_rule_on;
    polygon_rule_on.append(polygon_symbolizer(color(211, 211, 211)));
    polygon_style.add_rule(polygon_rule_on);
    map_->insert_style("default_face",polygon_style);
    emit new_style_added(dm_layer, "default_face");

    //Add default styles
    layer lyr(dm_layer.toStdString());
    lyr.set_datasource(ds);
    lyr.add_style("default_edge");
    lyr.add_style("default_face");
    map_->addLayer(lyr);

   this->drawMap();
}

void GUIMapnikView::editStyleDefintionGUI(QString layer_name)
{
    std::vector<layer> layers = map_->layers();

    QStringList styleNames;
    foreach (layer l, layers) {
        if (l.name() == layer_name.toStdString()) {
            std::vector<std::string> styles = l.styles();
            foreach (std::string s, styles)
                styleNames << QString::fromStdString(s);
            break;
        }
    }
    GUIStyleDefinition * gsd = new GUIStyleDefinition(layer_name, styleNames, this);
    connect (gsd, SIGNAL(removeStyle(QString,QString)), this, SLOT(removeStyleDefinition(QString,QString)));
    gsd->show();
}

void GUIMapnikView::removeStyleDefinition(QString layer_name, QString stylename)
{
    int index_layer = this->getLayerIndex(layer_name.toStdString());

    layer currentL = map_->getLayer(index_layer);

    layer new_layer(layer_name.toStdString());
    foreach (std::string cs, currentL.styles()) {
        if (cs == stylename.toStdString()) continue;
        new_layer.add_style(cs);
    }

    new_layer.set_datasource(currentL.datasource());
    map_->removeLayer(index_layer);
    map_->addLayer(new_layer);

    DM::Logger(DM::Debug) << "Remove Layer";

    emit removedStyle(layer_name, stylename);

    this->drawMap();

}

int GUIMapnikView::getLayerIndex(string layer_name)
{
    std::vector<layer> layers = map_->layers();
    for (uint i = 0; i < layers.size(); i++) {
        if (layers[i].name() == layer_name) return i;
    }
    return -1;
}

//polygon_rule_on.set_filter((parse_expression("[baujahr] = '2000'")));
