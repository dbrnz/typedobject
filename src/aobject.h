#ifndef _AOBJECT_H
#define _AOBJECT_H

#include "rdf/rdf.h"


#define A_OBJECT(...)                   \
 private:                               \
  static const rdf::URI m_metaclass;
#define PROPERTY_MAP(variable, property, ...)


namespace AObject {

  class AObject
  /*==========*/
  {
    A_OBJECT()
    PROPERTY_MAP(label, RDFS::label);
    PROPERTY_MAP(creator, DCT.creator, to_rdf=PropertyMap::get_uri);
  /*
    mapping = { 'label':       PropertyMap(RDFS.label),
                'comment':     PropertyMap(RDFS.comment),
                'description': PropertyMap(DCT.description),
                'precededBy':  PropertyMap(PRV.precededBy),
                'creator':     PropertyMap(DCT.creator, to_rdf=PropertyMap.get_uri),
                'created':     PropertyMap(DCT.created, datatype=XSD.dateTime,
                                           to_rdf=utils.datetime_to_isoformat,
                                           from_rdf=utils.isoformat_to_datetime),
  */
   public:
    AObject() ;
    AObject(const rdf::URI &p_uri) ;
    virtual ~AObject() ;

    /**
    Set attributes from RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    void addMetadata(const rdf::Graph &p_graph) ;

    /**
    Create a new instance of a resource, setting attributes from RDF triples in a graph.

    :param uri: The URI for the resource.
    :param graph: A RDF graph.
    :type graph: :class:`~biosignalml.rdf.Graph`
    :rtype: :class:`AbstractObject` or a sub-class.
    **/
    static AObject *createFromGraph(const rdf::Graph &p_graph, const rdf::URI &p_uri) ;

    void setGraph(const rdf::URI &p_graph) ;


   private:
    const rdf::URI m_uri ;

    std::string m_label ;        // Does this come via PROPERTY_MAP define??
    } ;

  } ;

#endif
