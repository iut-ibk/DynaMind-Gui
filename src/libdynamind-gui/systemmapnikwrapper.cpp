#include "systemmapnikwrapper.h"


#include <boost/make_shared.hpp>
#include <dmsystem.h>

using mapnik::datasource;
using mapnik::parameters;


DATASOURCE_PLUGIN(SystemMapnikWrapper)

SystemMapnikWrapper::SystemMapnikWrapper(mapnik::parameters const & params, bool bind,  DM::System * sys )
: datasource(params),
  sys(sys),
    desc_(*params.get<std::string>("type"), *params.get<std::string>("encoding","utf-8")),
    extent_()
{
    this->init(params);
}

void SystemMapnikWrapper::init(mapnik::parameters const& params)
{
    // every datasource must have some way of reporting its extent
    // in this case we are not actually reading from any data so for fun
    // let's just create a world extent in Mapnik's default srs:
    // '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs' (equivalent to +init=epsg:4326)
    // see http://spatialreference.org/ref/epsg/4326/ for more details
    extent_.init(-180,-90,180,90);
}

SystemMapnikWrapper::~SystemMapnikWrapper() { }

// This name must match the plugin filename, eg 'hello.input'
const char * SystemMapnikWrapper::name()
{
    return "hello";
}

mapnik::datasource::datasource_t SystemMapnikWrapper::type() const
{
    return datasource::Vector;
}

mapnik::box2d<double> SystemMapnikWrapper::envelope() const
{
    return extent_;
}

boost::optional<mapnik::datasource::geometry_t> SystemMapnikWrapper::get_geometry_type() const
{
    return mapnik::datasource::Point;
}

mapnik::layer_descriptor SystemMapnikWrapper::get_descriptor() const
{
    return desc_;
}

mapnik::featureset_ptr SystemMapnikWrapper::features(mapnik::query const& q) const
{
    // if the query box intersects our world extent then query for features
    if (extent_.intersects(q.get_bbox()))
    {
        return boost::make_shared<SystemMapnikFeatureset>(q.get_bbox(),desc_.get_encoding(), sys);
    }

    // otherwise return an empty featureset pointer
    return mapnik::featureset_ptr();
}

mapnik::featureset_ptr SystemMapnikWrapper::features_at_point(mapnik::coord2d const& pt) const
{
    // features_at_point is rarely used - only by custom applications,
    // so for this sample plugin let's do nothing...
    return mapnik::featureset_ptr();
}
