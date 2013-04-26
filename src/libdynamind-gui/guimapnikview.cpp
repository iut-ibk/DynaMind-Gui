#include "guimapnikview.h"
#include "ui_guimapnikview.h"

//QT
#include <QPainter>

//DM
#include "systemmapnikwrapper.h"
#include "dmlogger.h"

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

using namespace mapnik;

GUIMapnikView::GUIMapnikView(DM::System * sys, QWidget *parent) :
    sys_(sys),
    QWidget(parent),
    ui(new Ui::GUIMapnikView)
{
    ui->setupUi(this);
    this->init_mapnik();
    this->drawMap();
}

GUIMapnikView::~GUIMapnikView()
{
    delete ui;
}

void GUIMapnikView::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);

    painter.drawPixmap(QPoint(0, 0),pix_);
    painter.end();
}

void GUIMapnikView::init_mapnik() {

    std::string mapnik_dir("/usr/local/Cellar/mapnik/2.1.0/lib");
    DM::Logger(DM::Debug) << " looking for 'shape.input' plugin in... " << mapnik_dir << "/mapnik/input/";
    datasource_cache::instance()->register_datasources(mapnik_dir + "/mapnik/input/");
    DM::Logger(DM::Debug) << " looking for DejaVuSans font in... " << mapnik_dir << "/mapnik/fonts/DejaVuSans.ttf";
    freetype_engine::register_font(mapnik_dir + "/mapnik/fonts/DejaVuSans.ttf");


    map_ = new Map(this->width(),this->height());

    DM::Logger(DM::Debug) << this->width();
    map_->set_background(color("white"));


    // Provinces (polygon)
    feature_type_style provpoly_style;

    rule provpoly_rule_on;
    provpoly_rule_on.append(stroke(color(0, 0, 0), 10.0));
    provpoly_style.add_rule(provpoly_rule_on);

    map_->insert_style("cityblocks",provpoly_style);

    // Provinces (polygon)
    feature_type_style polygon_style;

    rule polygon_rule_on;
    polygon_rule_on.append(polygon_symbolizer(color(0, 0, 0)));
    polygon_style.add_rule(polygon_rule_on);

    map_->insert_style("superblock",polygon_style);

    // layers
    // Provincial  polygons
    {
        parameters p;
        p["type"]="dm";
        //p["file"]="/Users/christianurich/Documents/innsbruck_cityblocks_faces";
        //p["encoding"]="latin1";

        layer lyr("key");
        boost::shared_ptr<SystemMapnikWrapper> ds(new SystemMapnikWrapper(p, true, sys_));
        lyr.set_datasource(ds);
        lyr.add_style("superblock");
        map_->addLayer(lyr);
    }

}

void GUIMapnikView::drawMap()
{
    map_->zoom_all();
    image_32 buf(map_->width(),map_->height());
    agg_renderer<image_32> ren(*map_,buf);
    ren.apply();

    QImage image((uchar*)buf.raw_data(),map_->width(),map_->height(),QImage::Format_ARGB32);
    pix_ = QPixmap::fromImage(image.rgbSwapped());
}
