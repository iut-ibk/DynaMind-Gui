#include "viewer2d_prototype.h"
#include "systemmapnikwrapper.h"
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

#if defined(HAVE_CAIRO)
#include <mapnik/cairo_renderer.hpp>
#include <mapnik/cairo_context.hpp>
#endif

#include <iostream>


viewer2d_prototype::viewer2d_prototype()
{

    double offsetx = -10;
    double offsety = -10;

    double width = 20;
    double height = 20;

    DM::System sys;

    DM::Node * n1 = sys.addNode(offsetx,offsety,0);
    DM::Node * n2 = sys.addNode(offsetx+width,offsety,0);
    DM::Node * n3 = sys.addNode(offsetx+width,offsety+height,0);
    DM::Node * n4 = sys.addNode(offsetx,offsety+height,0);

    std::vector<DM::Node*> ve;
    ve.push_back(n1);
    ve.push_back(n2);
    ve.push_back(n3);
    ve.push_back(n4);
    ve.push_back(n1);

    sys.addFace(ve, DM::View("SUPERBLOCK", DM::FACE, DM::WRITE));

    using namespace mapnik;
    const std::string srs_lcc="+proj=tmerc +lat_0=0 +lon_0=10.33333333333333 +k=1 +x_0=0 +y_0=-5000000 +ellps=bessel +towgs84=577.326,90.129,463.919,5.137,1.474,5.297,2.4232 +units=m +no_defs";
    //const std::string srs_merc="+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0.0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs +over";

    std::cout << " running demo ... \n";
    std::string mapnik_dir("/usr/local/Cellar/mapnik/2.1.0/lib");

    std::cout << " looking for 'shape.input' plugin in... " << mapnik_dir << "/mapnik/input/" << "\n";
    datasource_cache::instance()->register_datasources(mapnik_dir + "/mapnik/input/");
    std::cout << " looking for DejaVuSans font in... " << mapnik_dir << "/mapnik/fonts/DejaVuSans.ttf" << "\n";
    freetype_engine::register_font(mapnik_dir + "/mapnik/fonts/DejaVuSans.ttf");
    std::cout << "alive" << std::endl;


    Map m(800,600);
    m.set_background(color("white"));
    m.set_srs(srs_lcc);
    // create styles

    // Provinces (polygon)
    feature_type_style provpoly_style;

    rule provpoly_rule_on;
    provpoly_rule_on.append(stroke(color(0, 0, 0), 10.0));
    provpoly_style.add_rule(provpoly_rule_on);

    m.insert_style("cityblocks",provpoly_style);

    // Provinces (polygon)
    feature_type_style polygon_style;

    rule polygon_rule_on;
    polygon_rule_on.append(polygon_symbolizer(color(0, 0, 0)));
    polygon_style.add_rule(polygon_rule_on);

    m.insert_style("superblock",polygon_style);

    // layers
    // Provincial  polygons
    {
        parameters p;
        p["type"]="dm";
        //p["file"]="/Users/christianurich/Documents/innsbruck_cityblocks_faces";
        //p["encoding"]="latin1";

        layer lyr("key");
        boost::shared_ptr<SystemMapnikWrapper> ds(new SystemMapnikWrapper(p, true, &sys));
        lyr.set_datasource(ds);
        //lyr.add_style("cityblocks");
        lyr.add_style("superblock");
        lyr.set_srs(srs_lcc);
        m.addLayer(lyr);
    }
    box2d<double> bb;
    bb = m.getLayer(0).envelope();
    std::cout << bb.height() << std::endl;
    std::cout << bb.width() << std::endl;

    m.zoom_all();
    image_32 buf(m.width(),m.height());
    agg_renderer<image_32> ren(m,buf);
    ren.apply();

    save_to_file(buf,"demo.jpg","jpeg");
    save_to_file(buf,"demo.png","png");
    save_to_file(buf,"demo256.png","png8");
    save_to_file(buf,"demo.tif","tiff");

    std::cout << "Three maps have been rendered using AGG in the current directory:\n"
                 "- demo.jpg\n"
                 "- demo.png\n"
                 "- demo256.png\n"
                 "- demo.tif\n"
                 "Have a look!\n" << std::endl;

}
