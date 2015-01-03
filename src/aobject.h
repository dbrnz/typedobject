#ifndef AOBJECT_H
#define AOBJECT_H

#include "rdf.h"
#include "utils.h"

#include <string>


#ifdef AOC_COMPILE

int _PARAMETERS_1(const char *property) { return 0 ; }
int _PARAMETERS_2(const char *name, const char *property, ...) { return 0 ; }
int _PARAMETERS_3(const char *name, const char *property, ...) { return 0 ; }
#define A_OBJECT                          \
  static int _AOBJECT_DEFINITION = 0 ;
#define METACLASS(CLASS)                  \
  static int _PROPERTY_METACLASS = _PARAMETERS_1(#CLASS) ;
#define PROPERTY_STRING(NAME, P, ...)     \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("STRING",   #P, #__VA_ARGS__) ;
#define PROPERTY_INTEGER(NAME, P, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("INTEGER",  #P, #__VA_ARGS__) ;
#define PROPERTY_DOUBLE(NAME, P, ...)     \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("DOUBLE",   #P, #__VA_ARGS__) ;
#define PROPERTY_NODE(NAME, P, ...)       \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("NODE",     #P, #__VA_ARGS__) ;
#define PROPERTY_URI(NAME, P, ...)        \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("URI",      #P, #__VA_ARGS__) ;
#define PROPERTY_DATETIME(NAME, P, ...)   \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("DATETIME", #P, #__VA_ARGS__) ;
#define PROPERTY_DURATION(NAME, P, ...)   \
  static int _PROPERTY_##NAME##  = _PARAMETERS_2("DURATION", #P, #__VA_ARGS__) ;
#define PROPERTY_AOBJECT(T, NAME, P, ...)   \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3("AOBJECT",  #P, #T, #__VA_ARGS__) ;

#else

#define A_OBJECT                          \
 protected:                               \
  void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property, const rdf::Node &value) ;

#define METACLASS(CLASS)                  \
 public:                                  \
  const rdf::URI &metaclass(void) const ;


#define PROPERTY(NAME, T)                 \
 public:                                  \
  inline const T & NAME(void) const       \
    { return m_##NAME ; }                 \
  inline void set_##NAME(const T & value) \
    { m_##NAME = value ; }                \
 private:                                 \
  T m_##NAME ;

#define PROPERTY_STRING(NAME, P, ...)     PROPERTY(NAME, std::string)
#define PROPERTY_INTEGER(NAME, P, ...)    PROPERTY(NAME, long)
#define PROPERTY_DOUBLE(NAME, P, ...)     PROPERTY(NAME, double)
#define PROPERTY_NODE(NAME, P, ...)       PROPERTY(NAME, rdf::Node)
#define PROPERTY_URI(NAME, P, ...)        PROPERTY(NAME, rdf::URI)
#define PROPERTY_DATETIME(NAME, P, ...)   PROPERTY(NAME, utils::Datetime)
#define PROPERTY_DURATION(NAME, P, ...)   PROPERTY(NAME, utils::Duration)
#define PROPERTY_AOBJECT(T, NAME, P, ...) PROPERTY(NAME, T)

#endif


namespace AObject
/*=============*/
{

  class AObject
  /*---------*/
  {
   protected:
    virtual void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property, const rdf::Node &value) = 0 ;

   public:
    virtual const rdf::URI &metaclass(void) const = 0 ;
    inline const rdf::URI &uri() const { return m_uri ; }

   public:
    AObject() ;
    AObject(const std::string &uri) ;
//    AObject(const std::string &uri, const rdf::Graph &graph) ;

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
