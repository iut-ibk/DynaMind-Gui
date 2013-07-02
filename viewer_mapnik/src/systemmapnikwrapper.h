#ifndef SYSTEMMAPNIKWRAPPER_H
#define SYSTEMMAPNIKWRAPPER_H

#include <dmsystem.h>

// mapnik
#include <mapnik/datasource.hpp>
#include <mapnik/params.hpp>
#include <mapnik/query.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/box2d.hpp>
#include <mapnik/coord.hpp>
#include <mapnik/feature_layer_desc.hpp>

#include "systemmapnikfeatureset.h"

// boost
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>


class SystemMapnikWrapper : public mapnik::datasource
{
public:
    SystemMapnikWrapper(mapnik::parameters const &param,  bool bind=true, DM::System * sys = 0);

    virtual ~SystemMapnikWrapper();


    // mandatory: type of the plugin, used to match at runtime
    mapnik::datasource::datasource_t type() const;

    // mandatory: name of the plugin
    static const char * name();

    // mandatory: function to query features by box2d
    // this is called when rendering, specifically in feature_style_processor.hpp
    mapnik::featureset_ptr features(mapnik::query const& q) const;

    // mandatory: function to query features by point (coord2d)
    // not used by rendering, but available to calling applications
    mapnik::featureset_ptr features_at_point(const mapnik::coord2d &pt,  double tol = 0) const;

    // mandatory: return the box2d of the datasource
    // called during rendering to determine if the layer should be processed
    mapnik::box2d<double> envelope() const;

    // mandatory: optionally return the overal geometry type of the datasource
    boost::optional<mapnik::datasource::geometry_t> get_geometry_type() const;

    // mandatory: return the layer descriptor
    mapnik::layer_descriptor get_descriptor() const;


    std::string getSourceType() {return this->source_type;}
    std::string getViewName() {return this->view_name;}
    std::string getViewType() {return this->view_type;}

private:
    // recommended - do intialization in a so-named init function
    // to reduce code in constructor
    void init(mapnik::parameters const& params);
    // recommended naming convention of datasource members:
    // name_, type_, extent_, and desc_
    static const std::string name_;
    mapnik::layer_descriptor desc_;
    mapnik::box2d<double> extent_;
    DM::System * sys;
    DM::View view;

    std::string source_type;
    std::string view_name;
    std::string view_type;




};

#endif // SYSTEMMAPNIKWRAPPER_H
