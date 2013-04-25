#ifndef SYSTEMMAPNIKFEATURESET_H
#define SYSTEMMAPNIKFEATURESET_H

#include <dmsystem.h>

// mapnik
#include <mapnik/datasource.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/unicode.hpp>

// boost
#include <boost/scoped_ptr.hpp> // needed for wrapping the transcoder

class SystemMapnikFeatureset : public mapnik::Featureset
{
public:
    SystemMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys);

    // desctructor
    virtual ~SystemMapnikFeatureset();

    // mandatory: you must expose a next() method, called when rendering
    mapnik::feature_ptr next();

private:
    // members are up to you, but these are recommended
    mapnik::box2d<double> box_;
    long feature_id_;
    boost::scoped_ptr<mapnik::transcoder> tr_;
    mapnik::context_ptr ctx_;
    DM::System* sys;
    std::vector<std::string> feature_uuids;

};

#endif // SYSTEMMAPNIKFEATURESET_H
