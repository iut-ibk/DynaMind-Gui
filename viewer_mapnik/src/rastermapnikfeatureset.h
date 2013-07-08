#ifndef RASTERMAPNIKFEATURESET_H
#define RASTERMAPNIKFEATURESET_H

#include <mapnik/datasource.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/unicode.hpp>
#include <mapnik/raster.hpp>


// boost
#include <boost/scoped_ptr.hpp> // needed for wrapping the transcoder
#include <boost/make_shared.hpp>

#include <dm.h>
class RasterMapnikFeatureset :  public mapnik::Featureset
{
public:
    RasterMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys, const DM::View & v);
    virtual ~RasterMapnikFeatureset();
    mapnik::feature_ptr next();

private:
    mapnik::box2d<double> box_;
    long feature_id_;
    boost::scoped_ptr<mapnik::transcoder> tr_;
    mapnik::context_ptr ctx_;
    DM::System* sys;
    DM::View view;


    DM::RasterData * rdata;
    mapnik::raster_ptr raster;


};

#endif // RASTERMAPNIKFEATURESET_H
