// mapnik
#include <mapnik/feature_factory.hpp>
#include <mapnik/geometry.hpp>

// boost
#include <boost/make_shared.hpp>

#include "systemmapnikfeatureset.h"

#include <dm.h>

SystemMapnikFeatureset::SystemMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys)
    : box_(box),
      feature_id_(0),
      tr_(new mapnik::transcoder(encoding)),
      sys(sys),
      ctx_(boost::make_shared<mapnik::context_type>())
{
    feature_uuids = sys->getUUIDs(DM::View("SUPERBLOCK", DM::FACE, DM::READ));


}

SystemMapnikFeatureset::~SystemMapnikFeatureset() { }

mapnik::feature_ptr SystemMapnikFeatureset::next()
{



    if (feature_id_ < feature_uuids.size())
    {
        // let us pretend it just has one column/attribute name
        /*std::string attribute("key");

        // the featureset context needs to know the field schema
        ctx_->push(attribute);

        // create a new feature
        mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx_,feature_id_));

        // increment the count so that we only return one feature


        // create an attribute pair of key:value
        UnicodeString ustr = tr_->transcode("hello world!");
        feature->put(attribute,ustr);

        // we need a geometry to display so just for fun here
        // we take the center of the bbox that was used to query
        // since we don't actually have any data to pull from...
        mapnik::coord2d center = box_.center();

        // create a new point geometry
        mapnik::geometry_type * pt = new mapnik::geometry_type(mapnik::Point);

        // we use path type geometries in Mapnik to fit nicely with AGG and Cairo
        // here we stick an x,y pair into the geometry using move_to()
        pt->move_to(center.x,center.y);

        // add the geometry to the feature
        feature->add_geometry(pt);

        // A feature usually will have just one geometry of a given type
        // but mapnik does support many geometries per feature of any type
        // so here we draw a line around the point
        mapnik::geometry_type * line = new mapnik::geometry_type(mapnik::LineString);
        line->move_to(box_.minx(),box_.miny());
        line->line_to(box_.minx(),box_.maxy());
        line->line_to(box_.maxx(),box_.maxy());
        line->line_to(box_.maxx(),box_.miny());
        line->line_to(box_.minx(),box_.miny());
        feature->add_geometry(line);*/

        std::string uuid = this->feature_uuids[feature_id_];

        DM::Face * f = sys->getFace(uuid);
        std::vector<DM::Node*> nodes = f->getNodePointers();

        mapnik::geometry_type * polygon = new mapnik::geometry_type(mapnik::Polygon);
        polygon->move_to(nodes[0]->getX(), nodes[0]->getY());
        for (int i = 1; i < nodes.size(); i++)
            polygon->line_to(nodes[i]->getX(), nodes[i]->getY());
        polygon->line_to(nodes[0]->getX(), nodes[0]->getY());


        ++feature_id_;

        mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx_,feature_id_));
        feature->add_geometry(polygon);
        // return the feature!
        return feature;
    }

    // otherwise return an empty feature
    return mapnik::feature_ptr();
}
