#include "guimapnikview.h"
#include "ui_guimapnikview.h"

//QT
#include <QPainter>
#include <QColor>
#include <QRgb>
#include <QWheelEvent>
#include <QMouseEvent>

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
    d(new mapnik_private),
    zoom_level(1.),
    pan(0,0),
    startPos(0,0)
{
    ui->setupUi(this);

    std::string mapnik_dir("/usr/local/Cellar/mapnik/2.2.0/lib");
    //DM::Logger(DM::Debug) << " looking for 'shape.input' plugin in... " << mapnik_dir << "/mapnik/input/";
    //datasource_cache::instance().register_datasource(mapnik_dir + "/mapnik/input/");
    DM::Logger(DM::Debug) << " looking for DejaVuSans font in... " << mapnik_dir << "/mapnik/fonts/DejaVuSans.ttf";
    freetype_engine::register_font(mapnik_dir + "/mapnik/fonts/DejaVuSans.ttf");

    connect(this, SIGNAL(zoom_in(double)), this, SLOT(increaseZoomLevel(double)));
    connect(this, SIGNAL(zoom_out(double)), this, SLOT(decreaseZoomLevel(double)));

    pan = QPoint(this->height()/2, this->width()/2);

    map_ = 0;



}

QString GUIMapnikView::getFilterForStyle(QString style)
{
    return this->styles_structs[style].filter;
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

void GUIMapnikView::wheelEvent(QWheelEvent *ev)
{
    if (ev->delta() > 0) {
        double factor = ev->delta() / 18. / 15.;
        emit zoom_out(factor);

    } else {
        double factor = ev->delta() / 18. / 15. * -1.;
        emit zoom_in(factor);

    }

    ev->accept();
}

void GUIMapnikView::mousePressEvent(QMouseEvent *ev)
{
    startPos = ev->pos();
}

void GUIMapnikView::mouseMoveEvent(QMouseEvent *ev)
{

    this->pan += (startPos - ev->pos()) * this->zoom_level;
    this->drawMap();
    startPos = ev->pos();
    update();
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
    try {
        map_->set_height(this->height());
        map_->set_width(this->width());
        map_->zoom_all();
        map_->pan(pan.x(), pan.y());
        map_->zoom(this->zoom_level);
        image_32 buf(map_->width(),map_->height());
        agg_renderer<image_32> ren(*map_,buf);
        ren.apply();

        QImage image((uchar*)buf.raw_data(),map_->width(),map_->height(),QImage::Format_ARGB32);
        pix_ = QPixmap::fromImage(image.rgbSwapped());
    }
    catch ( const mapnik::config_error & ex )
    {
        DM::Logger(DM::Error) << "### Configuration error: " << ex.what();
    }
    catch ( const std::exception & ex )
    {
        DM::Logger(DM::Error) <<  "### std::exception: " << ex.what();
    }
    catch ( ... )
    {
        DM::Logger(DM::Error) <<  "### Unknown exception.";
    }
}

void GUIMapnikView::setSystem(DM::System * sys)
{
    this->sys_ = sys;
    this->init_mapnik();
    this->drawMap();
}

void GUIMapnikView::addDefaultLayer(layer & lyr, QString dm_layer)
{
    //Default symbolizer Edge
    feature_type_style edges_style;
    rule edge_rule_on("");
    edge_rule_on.append(stroke(color(0, 0, 0), 1));
    edges_style.add_rule(edge_rule_on);
    map_->insert_style("default_edge",edges_style);
    lyr.add_style("default_edge");
    emit new_style_added(dm_layer, "default_edge");

    //Default symbolizer Polygon

    feature_type_style polygon_style;
    rule polygon_rule_on("");
    polygon_rule_on.append(polygon_symbolizer(color(211, 211, 211)));
    polygon_style.add_rule(polygon_rule_on);
    map_->insert_style("default_face",polygon_style);
    emit new_style_added(dm_layer, "default_face");


    lyr.add_style("default_face");
}

void GUIMapnikView::addLayer(QString dm_layer, bool withdefault)
{
    try {
        parameters p;
        p["type"]="dm";
        p["view_name"]= dm_layer.toStdString();
        stringstream s_view_type;
        s_view_type << this->sys_->getViewDefinition(dm_layer.toStdString())->getType();
        p["view_type"]= s_view_type.str();
        boost::shared_ptr<SystemMapnikWrapper> ds(new SystemMapnikWrapper(p, true, sys_));

        emit new_layer_added(dm_layer);

        d->datasources_[dm_layer] = ds;

        //Add default styles
        layer lyr(dm_layer.toStdString());
        lyr.set_datasource(ds);

        if (withdefault) addDefaultLayer(lyr, dm_layer);

        map_->addLayer(lyr);

        this->drawMap();
        update();
    }
    catch ( const mapnik::config_error & ex )
    {
        DM::Logger(DM::Error) << "### Configuration error: " << ex.what();
    }
    catch ( const std::exception & ex )
    {
        DM::Logger(DM::Error) <<  "### std::exception: " << ex.what();
    }
    catch ( ... )
    {
        DM::Logger(DM::Error) <<  "### Unknown exception.";
    }

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
    //Get View embended in the system
    if (!sys_->getViewDefinition(layer_name.toStdString())) return;
    DM::Component * cmp = sys_->getViewDefinition(layer_name.toStdString())->getDummyComponent();
    std::map<std::string, DM::Attribute*> attrs_map =  cmp->getAllAttributes();
    QStringList attribute_list;
    for (std::map<std::string, DM::Attribute*>::const_iterator it = attrs_map.begin();
         it != attrs_map.end();
         ++it) {
        attribute_list.append(QString::fromStdString(it->first));
    }
    GUIStyleDefinition * gsd = new GUIStyleDefinition(layer_name, styleNames, attribute_list, this);
    connect (gsd, SIGNAL(removeStyle(QString,QString)), this, SLOT(removeStyleDefinition(QString,QString)));
    connect (gsd, SIGNAL(newStyle(style_struct)), this, SLOT(addNewStyle(style_struct)));
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

    this->drawMap();

    emit removedStyle(layer_name, stylename);
}

void GUIMapnikView::addNewStyle(style_struct ss)
{
    try {
        feature_type_style new_style;
        rule new_rule_on("");
        if (ss.symbolizer == "PolygonSymbolizer") {
            new_rule_on.append(polygon_symbolizer(color(ss.color.red(), ss.color.green(), ss.color.blue())));
        }
        if (ss.symbolizer == "LineSymbolizer") {
            new_rule_on.append(stroke(color(ss.color.red(), ss.color.green(), ss.color.blue()), ss.linewidth));
        }
        if (ss.symbolizer == "BuildingSymbolizer"){
            new_rule_on.append(building_symbolizer(color(ss.color.red(), ss.color.green(), ss.color.blue()),parse_expression(ss.buildingHeight.toStdString())));
        }
        if (!ss.filter.isEmpty()) new_rule_on.set_filter(parse_expression(ss.filter.toStdString()));

        new_style.add_rule(new_rule_on);
        //new_style.set_opacity(ss.opacity);

        bool added_style = map_->insert_style(ss.name.toStdString(),new_style);

        if (!added_style) std::cout << "Error" << std::endl;

        //update layer;

        int index_layer = this->getLayerIndex(ss.layer.toStdString());
        if (index_layer == -1){
            DM::Logger(DM::Error) << ss.layer.toStdString() << " doesn't exist";
            return;
        }
        layer l = map_->getLayer(index_layer);
        l.add_style(ss.name.toStdString());
        map_->removeLayer(index_layer);
        map_->addLayer(l);


        this->styles_structs[ss.name] = ss;
    }
    catch ( const mapnik::config_error & ex )
    {
        DM::Logger(DM::Error) << "### Configuration error: " << ex.what();
    }
    catch ( const std::exception & ex )
    {
        DM::Logger(DM::Error) <<  "### std::exception: " << ex.what();
    }
    catch ( ... )
    {
        DM::Logger(DM::Error) <<  "### Unknown exception.";
    }

    this->drawMap();

    emit new_style_added(ss.layer, ss.name);
}

void GUIMapnikView::saveToPicture(unsigned width, unsigned height, QString filename)
{
    if (!sys_)
        return;
    map_->set_width(width);
    map_->set_height(height);
    map_->zoom_all();
    map_->pan(pan.x(), pan.y());
    map_->zoom(this->zoom_level);
    image_32 buf(map_->width(),map_->height());
    agg_renderer<image_32> ren(*map_,buf);
    ren.apply();

    save_to_file(buf,filename.toStdString(),"png");


}

void GUIMapnikView::increaseZoomLevel(double factor)
{
    this->zoom_level /= (1.+(0.2*factor));
    this->drawMap();
    update();
}

void GUIMapnikView::decreaseZoomLevel(double factor)
{
    this->zoom_level *= (1.+(0.2*factor));
    this->drawMap();
    update();
}

void GUIMapnikView::changeSystem(DM::System *sys)
{
    int counter_l = map_->layer_count();

    std::vector<layer> new_layer;
    for (int i = 0; i < counter_l; i++) {
        parameters p;
        layer lyr(map_->getLayer(i));
        p["type"]="dm";
        p["view_name"] = lyr.name();
        //Bug fix int was causing a ugly carsh
        stringstream s_view_type;
        s_view_type << this->sys_->getViewDefinition(lyr.name())->getType();
        p["view_type"]= s_view_type.str();
        boost::shared_ptr<SystemMapnikWrapper> ds(new SystemMapnikWrapper(p, true, sys));

        lyr.set_datasource(ds);
        new_layer.push_back(lyr);
    }

    for (int i = counter_l; i > 0; i--)  map_->removeLayer(i-1);

    for (int i = 0; i < counter_l; i++) map_->addLayer(new_layer[i]);

    this->drawMap();
    update();


}

std::string GUIMapnikView::save_style_to_file()
{

    std::stringstream out;
    //Write Map
    {
        boost::optional<color> bg = map_->background();

        out << "<Map ";
        if (!bg) {
            out << "background-color=\""
                << "rgb("
                << (*bg).red() << ","
                << (*bg).green() << ","
                << (*bg).blue() << ")\" ";
        }
        out << "srs=\""
            << map_->srs()
            << "\">\n";
    }

    //Write Styles
    typedef std::map<std::string,feature_type_style> style_map;
    for (style_map::const_iterator it = map_->styles().begin(); it != map_->styles().end(); it++) {
        feature_type_style sft = it->second;
        out << "\t";
        out  << "<Style name =\""
             << it->first
             << "\" "
             << "opacity=\""
             << sft.get_opacity() <<"\">"
             << "\n";

        foreach (rule r, sft.get_rules()) {
            out << "\t\t";
            out  << "<Rule>\n";
            foreach (symbolizer s, r.get_symbolizers()) {
                out << "\t\t\t";
                if  ((s.type())  == typeid(line_symbolizer))  {
                    line_symbolizer ls (boost::get<line_symbolizer>(s));
                    color c = ls.get_stroke().get_color();
                    out << "<LineSymbolizer stroke=\"";
                    out << "rgb("
                        << c.red() << ","
                        << c.green() << ","
                        << c.blue() << ")\" ";
                    out << "stroke-width=\""
                        << ls.get_stroke().get_width();
                    out << "\"/>\n";
                }
                if  ((s.type())  == typeid(polygon_symbolizer))  {
                    polygon_symbolizer ps (boost::get<polygon_symbolizer>(s));
                    color c = ps.get_fill();
                    out << "<PolygonSymbolizer fill=\"";
                    out << "rgb("
                        << c.red() << ","
                        << c.green() << ","
                        << c.blue() << ")\""
                        << "/>\n";
                }
                if  ((s.type())  == typeid(building_symbolizer))  {
                    building_symbolizer ps (boost::get<building_symbolizer>(s));
                    color c = ps.get_fill();
                    out << "<BuildingSymbolizer fill=\"";
                    out << "rgb("
                        << c.red() << ","
                        << c.green() << ","
                        << c.blue() << ")\" ";
                    out << "height=\""
                        << this->styles_structs[QString::fromStdString(it->first)].buildingHeight.toStdString()
                        << "\"/>\n";
                }
                out << "\t\t";
                out  << "</Rule>\n";
            }

            //Replace <> in filter
            QString filter = this->getFilterForStyle(QString::fromStdString(it->first));
            filter.replace(">", "&gt;");
            filter.replace("<", "&lt;");
            out << "<Filter value=\""
                << filter.toStdString()
                << "\"/>\n";

            out << "\t";
            out  << "</Style>\n";

        }

    }


    //Write Layers
    out << "\t";
    {
        foreach (layer l, map_->layers()) {
            out << "<Layer name=\""
                << l.name() << "\""
                << " srs=\""
                << l.srs()
                << "\">\n";
            foreach (std::string n, l.styles()) {
                out << "\t\t";
                out << "<Stylename";
                out << " name=\"style\">";
                out << n;
                out << "</Stylename>\n";
            }

            boost::shared_ptr<SystemMapnikWrapper> d = boost::static_pointer_cast<SystemMapnikWrapper>(l.datasource());
            out << "\t\t";
            out << "<Datasource>";
            out << "\n\t\t\t";
            out << "<Parameter name=\"type\">";
            out << d->getSourceType();
            out << "</Parameter>";
            out << "\n\t\t\t";
            out << "<Parameter name=\"view_name\">";
            out << d->getViewName();
            out << "</Parameter>";
            out << "\n\t\t\t";
            out << "<Parameter name=\"viewtype\">";
            out << d->getViewType();
            out << "</Parameter>\n";
            out << "\t\t";
            out << "</Datasource>\n";
        }
        out << "\t";
        out << "</Layer>\n";
    }

    out << "</Map>\n";

    return out.str();
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
