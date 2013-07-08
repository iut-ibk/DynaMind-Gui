#include "rastermapnikfeatureset.h"

// mapnik
#include <mapnik/feature_factory.hpp>
#include <mapnik/ctrans.hpp>
#include <mapnik/raster.hpp>

// boost
#include <boost/make_shared.hpp>

//dm
#include <tbvectordata.h>

RasterMapnikFeatureset::RasterMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys, const DM::View & v)
    : box_(box),
      feature_id_(0),
      tr_(new mapnik::transcoder(encoding)),
      sys(sys),
      view(v),
      ctx_(boost::make_shared<mapnik::context_type>()),
      rdata(0)
{
    std::map<std::string, DM::Component*> components = sys->getAllComponentsInView(view);
    mforeach(DM::Component* c, components) {
        if(c->getType() == DM::RASTERDATA) {
            rdata = (DM::RasterData*)c;
        }
    }

    mapnik::box2d<double> bb;

    double x1;
    double y1;
    double x2;
    double y2;

    TBVectorData::GetViewExtend(sys, view, x1, y1, x2, y2);
    bb.init(x1,y1,x2,y2);

    raster = boost::make_shared<mapnik::raster>(bb, rdata->getWidth(), rdata->getHeight());
    double height = rdata->getHeight();
    for (unsigned x = 0; x <  rdata->getWidth(); x++) {
        for (unsigned y = 0; y < rdata->getHeight(); y++) {
            float f_val = (float)rdata->getCell(x,y);
            raster->data_.setRow(height-y-1,x,x+1, reinterpret_cast<unsigned*>(&f_val));
        }
    }
}

RasterMapnikFeatureset::~RasterMapnikFeatureset()
{
}

mapnik::feature_ptr RasterMapnikFeatureset::next()
{
    if (feature_id_ != 0 || rdata == 0)
        return mapnik::feature_ptr();

    mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx_,feature_id_++));

    try
    {
        int image_width = rdata->getWidth();
        int image_height = rdata->getHeight();

        if (image_width > 0 && image_height > 0)
        {
            //mapnik::CoordTransform t(image_width, image_height, box_, 0, 0);
            //mapnik::box2d<double> intersect = box_.intersect(curIter_->envelope());
            //mapnik::box2d<double> ext = t.forward(intersect);
            //mapnik::box2d<double> rem = policy_.transform(ext);
            //if (ext.width() > 0.5 && ext.height() > 0.5 )
            //{
            // select minimum raster containing whole ext
            /*int x_off = static_cast<int>(std::floor(ext.minx()));
                int y_off = static_cast<int>(std::floor(ext.miny()));
                int end_x = static_cast<int>(std::ceil(ext.maxx()));
                int end_y = static_cast<int>(std::ceil(ext.maxy()));

                // clip to available data
                if (x_off < 0)
                    x_off = 0;
                if (y_off < 0)
                    y_off = 0;
                if (end_x > image_width)
                    end_x = image_width;
                if (end_y > image_height)
                    end_y = image_height;
                int width = end_x - x_off;
                int height = end_y - y_off;*/

            // calculate actual box2d of returned raster
            /*box2d<double> feature_raster_extent(rem.minx() + x_off,
                                                    rem.miny() + y_off,
                                                    rem.maxx() + x_off + width,
                                                    rem.maxy() + y_off + height);
                intersect = t.backward(feature_raster_extent);*/




            //raster->data_
            //reader->read(x_off, y_off, raster->data_);
            //raster->premultiplied_alpha_ = reader->premultiplied_alpha();

            feature->set_raster(raster);
            //}
        }
    }

    catch (std::exception const& ex)
    {
        DM::Logger(DM::Error) << "Raster Plugin: " << ex.what();
    }
    catch (...)
    {
        DM::Logger(DM::Error)  << "Raster Plugin: exception caught";
    }

    return feature;
}

