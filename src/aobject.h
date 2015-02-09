#ifndef AOBJECT_H
#define AOBJECT_H

#include "rdf.h"
#include "utils.h"

#include <string>
#include <set>


#ifdef AOC_COMPILE

int _PARAMETERS_1(const char *property) { return 0 ; }
int _PARAMETERS_2(const char *name, const char *property, ...) { return 0 ; }
int _PARAMETERS_3(const char *name, const char *property, ...) { return 0 ; }
#define TYPED_OBJECT(CLASS, TYPE)         \
  static int _OBJECT_DEFINITION = 0 ;     \
  static int _PROPERTY_METACLASS = _PARAMETERS_1(#TYPE) ;

#define _PROPERTY(NAME, P, T, ...)        \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, #__VA_ARGS__) ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "SET",  #__VA_ARGS__) ;

#define _PROPERTY_RSET(NAME, P, T, ...)   \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "RSET", #__VA_ARGS__) ;

#else

#define TYPED_OBJECT(CLASS, TYPE)         \
 protected:                               \
  void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,  \
                       const rdf::Node &value,  const bool reverse) ;       \
 public:                                  \
  CLASS() { }                             \
  CLASS(const std::string &uri) ;         \
  CLASS(const std::string &uri, const rdf::Graph &graph) ; \
                                          \
  const rdf::URI &metaclass(void) const ;

#define _PROPERTY(NAME, P, T, ...)        \
 public:                                  \
  inline const T & NAME(void) const       \
    { return m_##NAME ; }                 \
  inline void set_##NAME(const T & value) \
    { m_##NAME = value ; }                \
 private:                                 \
  T m_##NAME ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
 public:                                  \
  inline const std::set<T> & NAME(void) const \
    { return m_##NAME ; }                 \
  inline void add_##NAME(const T & value) \
    { m_##NAME.insert(value) ; }          \
 private:                                 \
  std::set<T> m_##NAME ;

#define _PROPERTY_RSET(NAME, P, T, ...)  _PROPERTY_SET(NAME, P, T)

#endif

#define PROPERTY_STRING(NAME, P)         _PROPERTY(NAME, P, std::string)
#define PROPERTY_INTEGER(NAME, P)        _PROPERTY(NAME, P, long)
#define PROPERTY_DOUBLE(NAME, P)         _PROPERTY(NAME, P, double)
#define PROPERTY_NODE(NAME, P)           _PROPERTY(NAME, P, rdf::Node)
#define PROPERTY_URI(NAME, P)            _PROPERTY(NAME, P, rdf::URI)
#define PROPERTY_DATETIME(NAME, P)       _PROPERTY(NAME, P, utils::Datetime)
#define PROPERTY_DURATION(NAME, P)       _PROPERTY(NAME, P, utils::Duration)
#define PROPERTY_OBJECT(NAME, P, T)      _PROPERTY(NAME, P, T, OBJECT)

#define PROPERTY_STRING_SET(NAME, P)     _PROPERTY_SET(NAME, P, std::string)
#define PROPERTY_NODE_SET(NAME, P)       _PROPERTY_SET(NAME, P, rdf::Node)
#define PROPERTY_URI_SET(NAME, P)        _PROPERTY_SET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_SET(NAME, P, T)  _PROPERTY_SET(NAME, P, T, OBJECT)

#define PROPERTY_URI_RSET(NAME, P)       _PROPERTY_RSET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_RSET(NAME, P, T) _PROPERTY_RSET(NAME, P, T, OBJECT)


namespace AObject
/*=============*/
{

  class AObject
  /*---------*/
  {
   protected:
    virtual void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,
                                 const rdf::Node &value,  const bool reverse) = 0 ;
   public:
    virtual const rdf::URI &metaclass(void) const = 0 ;
    inline const rdf::URI &uri() const { return m_uri ; }

   public:
    AObject() ;
    AObject(const std::string &uri) ;
//    AObject(const std::string &uri, const rdf::Graph &graph) ;

    virtual ~AObject() ;

    std::string asString(void) const ;

    bool operator==(const AObject &other) const ;

    bool operator<(const AObject &other) const ;

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
    rdf::URI m_uri ;
    } ;

  } ;

/**
  mapping = { 'label':       PropertyMap(RDFS.label),
              'comment':     PropertyMap(RDFS.comment),
              'description': PropertyMap(DCT.description),
              'precededBy':  PropertyMap(PRV.precededBy),
              'creator':     PropertyMap(DCT.creator, to_rdf=PropertyMap.get_uri),
              'created':     PropertyMap(DCT.created, datatype=XSD.dateTime,
                                         to_rdf=utils.datetime_to_isoformat,
                                         from_rdf=utils.isoformat_to_datetime),
            }
**/




#endif
