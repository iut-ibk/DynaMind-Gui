// mapnik
#include <mapnik/feature_factory.hpp>
#include <mapnik/geometry.hpp>

// boost
#include <boost/make_shared.hpp>

#include "systemmapnikfeatureset.h"

#include <dm.h>

SystemMapnikFeatureset::SystemMapnikFeatureset(mapnik::box2d<double> const& box, std::string const& encoding, DM::System * sys, const DM::View & v)
    : box_(box),
      feature_id_(0),
      tr_(new mapnik::transcoder(encoding)),
      sys(sys),
      view(v),
      ctx_(boost::make_shared<mapnik::context_type>())
{
    feature_uuids = sys->getUUIDs(view);

    //Get View embended in the system
    DM::View * vptr = sys->getViewDefinition(v.getName());
    if (!vptr) {
        DM::Logger(DM::Warning) << "View doesn't exist yet";
        return;
    }
    DM::Component * cmp = sys->getViewDefinition(v.getName())->getDummyComponent();
    std::map<std::string, DM::Attribute*> attrs_map =  cmp->getAllAttributes();

    for (std::map<std::string, DM::Attribute*>::const_iterator it = attrs_map.begin();
         it != attrs_map.end();
         ++it) {
        attribute_list.push_back(it->first);
    }



}

SystemMapnikFeatureset::~SystemMapnikFeatureset() { }

mapnik::feature_ptr SystemMapnikFeatureset::next()
{

    foreach(std::string attr_name, attribute_list) {
        ctx_->push(attr_name);
    }


    if (feature_id_ < feature_uuids.size())
    {
        std::string uuid = this->feature_uuids[feature_id_];
        ++feature_id_;
        mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx_,feature_id_));

        DM::Component * cmp = 0;

        if (!sys->getComponent(uuid)) {
            DM::Logger(DM::Warning) << "Component doesn t exist";
            return mapnik::feature_ptr();
        }

        switch (view.getType()) {
        case DM::NODE:
            cmp = (DM::Component *)  sys->getNode(uuid);
            draw_node( (DM::Node*) cmp, feature);
            break;
        case DM::FACE:
            cmp = (DM::Component *)  sys->getFace(uuid);
            draw_faces( (DM::Face*) cmp, feature);
            break;
        case DM::EDGE:
            cmp = (DM::Component *) sys->getEdge(uuid);
            draw_edges((DM::Edge*) cmp, feature);
            break;
        }
        this->add_attribute(cmp, feature);
        // return the feature!
        return feature;
    }

    // otherwise return an empty feature
    return mapnik::feature_ptr();
}

void SystemMapnikFeatureset::draw_faces(DM::Face * f, mapnik::feature_ptr feature)
{
    std::vector<DM::Node*> nodes = f->getNodePointers();
    mapnik::geometry_type * polygon = new mapnik::geometry_type(mapnik::Polygon);
    polygon->move_to(nodes[0]->getX(), nodes[0]->getY());
    for (uint i = 1; i < nodes.size(); i++)
        polygon->line_to(nodes[i]->getX(), nodes[i]->getY());
    polygon->line_to(nodes[0]->getX(), nodes[0]->getY());
    polygon->close_path();

    foreach (DM::Face * f,  f->getHolePointers()) {
        nodes = f->getNodePointers();
        polygon->move_to(nodes[0]->getX(), nodes[0]->getY());
        for (uint i = 1; i < nodes.size(); i++)
            polygon->line_to(nodes[i]->getX(), nodes[i]->getY());
        polygon->line_to(nodes[0]->getX(), nodes[0]->getY());
        polygon->close_path();
    }

    feature->add_geometry(polygon);

}


void SystemMapnikFeatureset::draw_edges(DM::Edge *e, mapnik::feature_ptr feature)
{
    mapnik::geometry_type * line = new mapnik::geometry_type(mapnik::LineString);
    line->move_to(e->getStartNode()->getX(), e->getStartNode()->getY());
    line->line_to(e->getEndNode()->getX(), e->getEndNode()->getY());
    feature->add_geometry(line);

}

void SystemMapnikFeatureset::draw_node(DM::Node *n, mapnik::feature_ptr feature)
{
    mapnik::geometry_type * node = new mapnik::geometry_type(mapnik::Point);
    node->move_to(n->getX(), n->getY());
    feature->add_geometry(node);

}

void SystemMapnikFeatureset::add_attribute(DM::Component *cmp, mapnik::feature_ptr feature)
{
    foreach(std::string attr_name, attribute_list) {
        feature->put(attr_name, cmp->getAttribute(attr_name)->getDouble());
    }
}
