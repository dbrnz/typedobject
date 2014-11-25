#ifndef _AOBJECT_H
#define _AOBJECT_H

#include "rdf/rdf.h"
#include "rdf/defs.h"

#include <string>
#include <list>

/**

  static const rdf::URI m_metaclass ;                           \
  inline const std::list<const rdf::URI> metaorder(void) {      \
    return { METACLASS, ##__VA_ARGS__ } ;                       \
    }

//    return std::list<const rdf::URI>( METACLASS, ##__VA_ARGS__ ) ;  \

// Each class has a static rdfmap and a list pointing
// to the rdfmaps of each ancestor.


    from_rdf(graph) {
      SUPER::from_rdf(graph) ;
      // self maps ...
      label = graph.get_object(m_uri, RDFS::label) ;
      // but does graph query (<m_uri>, <property>, ?value) for each assignment
      }


  v's having statement (m_uri, RDFS::label, value) and then "label = value".

  This requires a map from "RDFS::label" to (say) "set_label()" that is
  then called passing "value".

  However avoids multiple graph queries -- just the one that gets the list of
  statements (<m_uri>, ?property, ?value).


  Need to note both options

   What about:

    selfmaps[RDFS::label] = set_label ;

**/


/***
   public:
// Generated
    void from_rdf(const rdf::Graph p_graph &)
    {
//    PROPERTY_MAP(label, RDFS::label);
      label = graph.get_object(m_uri, RDFS::label) ;
//    PROPERTY_MAP(creator, DCT::creator, to_rdf=PropertyMap::get_uri);
      creator = graph.get_object(m_uri, DCT::CREATOR) ;

      xxx = #from_rdf#(graph.get_object(m_uri, #property#)) ;
      }

    void to_rdf(const rdf::Graph p_graph &)
    {
//    PROPERTY_MAP(label, RDFS::label);
      graph.add(m_uri, RDFS::label, label) ;
//    PROPERTY_MAP(creator, DCT::creator, to_rdf=PropertyMap::get_uri);
      graph.add(m_uri, DCT::CREATOR, PropertyMap::get_uri(creator)) ;
      }

  typedef void (*RdfMap)(const rdf::Graph &) ;

  static const std::list<RdfMap> m_to_rdf ;  // to_rdf functions for self and parent classes

***/




#define A_OBJECT(METACLASS, ...)                   \
 public:                                           \
  virtual const rdf::URI &metaclass(void) const {  \
    { static rdf::URI m_metaclass = METACLASS ;    \
      return METACLASS ;                           \
    }

#define PROPERTY_MAP(variable, property, ...)


namespace AObject
/*=============*/
{

  class AObject
  /*---------*/
  {
   public:
    virtual const rdf::URI &metaclass(void) const ;

   protected:
    virtual void assign_from_rdf(const rdf::Node &property, const rdf::Node &value) ;

//    PROPERTY_MAP(label, RDFS::label);
//    PROPERTY_MAP(creator, DCT::creator, to_rdf=PropertyMap::get_uri) ;

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

    inline const rdf::URI &uri() const { return m_uri ; }

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

// Does these come via PROPERTY_MAP defines??
    std::string label ;
    std::string comment ;
    std::string description ;
    rdf::URI precededBy ;
    rdf::URI creator ;
    uint64_t created ;   // ***** DATETIME type
    } ;


  } ;

#endif
