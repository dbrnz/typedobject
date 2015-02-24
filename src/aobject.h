#ifndef AOBJECT_H
#define AOBJECT_H

#include "rdf.h"
#include "utils.h"

#include <string>
#include <map>
#include <set>


#ifdef AOC_COMPILE

int _PARAMETERS_1(const char *property) { return 0 ; }
int _PARAMETERS_2(const char *name, const char *property, ...) { return 0 ; }
int _PARAMETERS_3(const char *name, const char *property, ...) { return 0 ; }
#define TYPED_OBJECT(CLASS, TYPE)         \
  static int _OBJECT_DEFINITION = 0 ;     \
  static int _PROPERTY_TYPE = _PARAMETERS_1(#TYPE) ;

#define _PROPERTY(NAME, P, T, ...)        \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, #__VA_ARGS__) ;

#define _PROPERTY_OBJ(NAME, P, T, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "OBJ",  #__VA_ARGS__) ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "SET",  #__VA_ARGS__) ;

#define _PROPERTY_OBJ_SET(NAME, P, T, ...)\
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "SET",  "OBJ", #__VA_ARGS__) ;

#define _PROPERTY_RSET(NAME, P, T, ...)   \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "RSET", #__VA_ARGS__) ;

#define _PROPERTY_OBJ_RSET(NAME, P, T, ...) \
  static int _PROPERTY_##NAME##  = _PARAMETERS_3(#T, #P, "RSET", "OBJ", #__VA_ARGS__) ;

#define _ASSIGN(NAME, P, T, ...)          \
  static int _ASSIGN_##NAME##    = _PARAMETERS_3(#T, #P, #__VA_ARGS__) ;

#else

#define TYPED_OBJECT(CLASS, TYPE)         \
 protected:                               \
  void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,  \
                       const rdf::Node &value,  const bool reverse) ;       \
 public:                                  \
  CLASS() { }                             \
  CLASS(const std::string &uri) ;         \
  CLASS(const std::string &uri, const rdf::Graph &graph) ; \
  ~CLASS() ;                                               \
  const rdf::URI &type(void) const ;                       \
  static std::set<rdf::URI> &subtypes(void) ;              \
  static int add_subtype(const rdf::URI &T) ;

#define _PROPERTY(NAME, P, T, ...)        \
 public:                                  \
  inline const T & NAME(void) const       \
    { return m_##NAME ; }                 \
  inline void set_##NAME(const T & value) \
    { m_##NAME = value ; }                \
 protected:                                 \
  T m_##NAME ;

#define _PROPERTY_OBJ(NAME, P, T, ...)    \
 public:                                  \
  inline const T * NAME(void) const       \
    { return m_##NAME ; }                 \
  inline void set_##NAME(T * value) \
    { m_##NAME = value ; }                \
 private:                                 \
  T * m_##NAME ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
 public:                                  \
  inline const std::set<T> & NAME(void) const \
    { return m_##NAME ; }                 \
  inline void add_##NAME(const T & value) \
    { m_##NAME.insert(value) ; }          \
 private:                                 \
  std::set<T> m_##NAME ;

#define _PROPERTY_OBJ_SET(NAME, P, T, ...)\
 public:                                  \
  inline const std::set<T *> & NAME(void) const \
    { return m_##NAME ; }                 \
  inline void add_##NAME(T * value) \
    { m_##NAME.insert(value) ; }          \
 private:                                 \
  std::set<T *> m_##NAME ;

#define _PROPERTY_RSET(NAME, P, T, ...)      _PROPERTY_SET(NAME, P, T)
#define _PROPERTY_OBJ_RSET(NAME, P, T, ...)  _PROPERTY_OBJ_SET(NAME, P, T)

#define _ASSIGN(NAME, P, T, ...)

#endif

#define PROPERTY_STRING(NAME, P)         _PROPERTY(NAME, P, std::string)
#define PROPERTY_INTEGER(NAME, P)        _PROPERTY(NAME, P, long)
#define PROPERTY_DOUBLE(NAME, P)         _PROPERTY(NAME, P, double)
#define PROPERTY_NODE(NAME, P)           _PROPERTY(NAME, P, rdf::Node)
#define PROPERTY_URI(NAME, P)            _PROPERTY(NAME, P, rdf::URI)
#define PROPERTY_DATETIME(NAME, P)       _PROPERTY(NAME, P, utils::Datetime)
#define PROPERTY_DURATION(NAME, P)       _PROPERTY(NAME, P, utils::Duration)
#define PROPERTY_OBJECT(NAME, P, T)      _PROPERTY_OBJ(NAME, P, T)

#define PROPERTY_STRING_SET(NAME, P)     _PROPERTY_SET(NAME, P, std::string)
#define PROPERTY_NODE_SET(NAME, P)       _PROPERTY_SET(NAME, P, rdf::Node)
#define PROPERTY_URI_SET(NAME, P)        _PROPERTY_SET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_SET(NAME, P, T)  _PROPERTY_OBJ_SET(NAME, P, T, OBJ)

#define PROPERTY_URI_RSET(NAME, P)       _PROPERTY_RSET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_RSET(NAME, P, T) _PROPERTY_OBJ_RSET(NAME, P, T, OBJ)

#define ASSIGN_DATETIME(NAME, P)         _ASSIGN(NAME, P, utils::Datetime)
#define ASSIGN_DURATION(NAME, P)         _ASSIGN(NAME, P, utils::Duration)


namespace AObject
/*=============*/
{

  class AObject ;

  class AObjectFactory
  /*----------------*/
  {
   public:
    virtual AObject *create(const std::string &uri) = 0 ;
    } ;

#define REGISTER_TYPE(T, CLS)                                 \
  class CLS##Factory : public AObject::AObjectFactory {       \
   public:                                                    \
    inline CLS##Factory() { AObject::AObject::register_type(T, this) ; } \
    virtual AObject::AObject *create(const std::string &uri) { return new CLS(uri) ; } \
    } ;                                                       \
  static CLS##Factory _global_##CLS##Factory ;                \
  static int _global_##CLS##_type = CLS::add_subtype(T) ;

#define REGISTER_SUBTYPE(T, CLS, BASE)                        \
  static int _global_##CLS##supertype = BASE::add_subtype(T) ;


  class AObject
  /*---------*/
  {
   protected:
    virtual void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,
                                 const rdf::Node &value,  const bool reverse) = 0 ;
   public:
    virtual const rdf::URI &type(void) const = 0 ;
    inline const rdf::URI &uri() const { return m_uri ; }
    static inline int add_subtype(const rdf::URI &T) { return 0 ; }

   public:
    AObject() ;
    AObject(const std::string &uri) ;
    AObject(const std::string &uri, const rdf::Graph &graph) ;

    virtual ~AObject() ;

    static AObject *create(const rdf::URI &T, const std::string &uri) ;

    template <class  T>
    static T *create(std::set<rdf::URI> &subtypes, const rdf::Node &uri, const rdf::Graph &graph)
    /*-----------------------------------------------------------------------------------------*/
    {
      rdf::StatementIter types = graph.getStatements(uri, rdf::RDF::type, rdf::Node()) ;
      if (!types.end()) {
        do {
          rdf::URI type = rdf::URI(types.get_object()) ;
          if (subtypes.find(type) != subtypes.end()) {
            AObject *obj = create(type, uri.to_string()) ;
            obj->add_metadata(graph) ;
            return dynamic_cast<T *>(obj) ;
            }
          } while (!types.next()) ;
        }
      return nullptr ;
      }


    std::string to_string(void) const ;

    bool operator==(const AObject &other) const ;

    bool operator<(const AObject &other) const ;

    static void register_type(const rdf::URI &T, AObjectFactory *factory) ;

    /**
    Set attributes from RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    bool add_metadata(const rdf::Graph &p_graph) ;

   private:
    rdf::URI m_uri ;
    static std::map<rdf::URI, AObjectFactory *> &m_factories(void) ;
    } ;

  } ;

#endif
