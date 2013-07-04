#ifndef SYSTEMMAPNIKFEATURESET_H
#define SYSTEMMAPNIKFEATURESET_H

#include <dm.h>

// mapnik
#include <mapnik/datasource.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/unicode.hpp>

// boost
#include <boost/scoped_ptr.hpp> // needed for wrapping the transcoder

class SystemMapnikFeatureset : public mapnik::Featureset
{
public:
    SystemMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys, const DM::View & v);

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
    DM::View view;
    void draw_faces(DM::Face *f,  mapnik::feature_ptr feature);
    void draw_edges(DM::Edge *e,  mapnik::feature_ptr feature);
    void draw_node(DM::Node *n,  mapnik::feature_ptr feature);
    void add_attribute(DM::Component * cmp,  mapnik::feature_ptr feature);

    std::vector<std::string> attribute_list;

};

#endif // SYSTEMMAPNIKFEATURESET_H
