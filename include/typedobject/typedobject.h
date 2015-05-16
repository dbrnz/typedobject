/*****************************************************************************
 *                                                                           *
 *  Typed RDF Objects for C++                                                *
 *                                                                           *
 *  Copyright (c) 2014-2015  David Brooks                                    *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef TYPEDOBJECT_TYPEDOBJECT_H
#define TYPEDOBJECT_TYPEDOBJECT_H

#include <typedobject/typedobject_export.h>
#include <typedobject/rdf.h>
#include <typedobject/rdfdefs.h>
#include <typedobject/xsd.h>

#include <string>
#include <map>
#include <set>


#ifdef TYPED_OBJECT_COMPILE

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

#define _RESTRICTION(NAME, VALUE, ...)   \
  static int _RESTRICT_##NAME##  = _PARAMETERS_2(#VALUE, #__VA_ARGS__) ;

#else

#define TYPED_OBJECT(CLASS, TYPE)         \
 protected:                               \
  void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,  \
                       const rdf::Node &value,  const bool reverse) ;       \
  void save_as_rdf(const rdf::Graph &graph) ;                               \
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
#define _RESTRICTION(NAME, VALUE, ...)

#endif

#define PROPERTY_STRING(NAME, P)         _PROPERTY(NAME, P, std::string)
#define PROPERTY_INTEGER(NAME, P)        _PROPERTY(NAME, P, rdf::Integer)
#define PROPERTY_DECIMAL(NAME, P)        _PROPERTY(NAME, P, rdf::Decimal)
#define PROPERTY_NODE(NAME, P)           _PROPERTY(NAME, P, rdf::Node)
#define PROPERTY_URI(NAME, P)            _PROPERTY(NAME, P, rdf::URI)
#define PROPERTY_DATETIME(NAME, P)       _PROPERTY(NAME, P, xsd::Datetime)
#define PROPERTY_DURATION(NAME, P)       _PROPERTY(NAME, P, xsd::Duration)
#define PROPERTY_OBJECT(NAME, P, T)      _PROPERTY_OBJ(NAME, P, T)

// What is the difference between URI, NODE, and OBJECT properties??
// Could URI and NODE be combined? And renamed to RESOURCE??

#define PROPERTY_STRING_SET(NAME, P)     _PROPERTY_SET(NAME, P, std::string)
#define PROPERTY_NODE_SET(NAME, P)       _PROPERTY_SET(NAME, P, rdf::Node)
#define PROPERTY_URI_SET(NAME, P)        _PROPERTY_SET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_SET(NAME, P, T)  _PROPERTY_OBJ_SET(NAME, P, T, OBJ)

#define PROPERTY_URI_RSET(NAME, P)       _PROPERTY_RSET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_RSET(NAME, P, T) _PROPERTY_OBJ_RSET(NAME, P, T, OBJ)

#define ASSIGN_DATETIME(NAME, P)         _ASSIGN(NAME, P, xsd::Datetime)
#define ASSIGN_DURATION(NAME, P)         _ASSIGN(NAME, P, xsd::Duration)

#define RESTRICTION(NAME, VALUE)         _RESTRICTION(NAME, VALUE)


namespace TypedObject
/*=================*/
{

  const std::string VERSION = "0.0.5" ;

#ifdef TYPED_OBJECT_COMPILE

  class TypedObject { } ;

#else

  class TYPEDOBJECT_EXPORT TypedObject ;

  class TYPEDOBJECT_EXPORT TypedObjectFactory
  /*---------------------------------------*/
  {
   public:
    virtual TypedObject *create(const std::string &uri) = 0 ;
    } ;

#define REGISTER_TYPE(T, CLS)                                 \
  class CLS##Factory : public TypedObject::TypedObjectFactory {       \
   public:                                                    \
    inline CLS##Factory() { TypedObject::TypedObject::register_type(T, this) ; } \
    virtual TypedObject::TypedObject *create(const std::string &uri) { return new CLS(uri) ; } \
    } ;                                                       \
  static CLS##Factory _global_##CLS##Factory ;                \
  static int _global_##CLS##_type = CLS::add_subtype(T) ;

#define REGISTER_SUBTYPE(T, CLS, BASE)                        \
  static int _global_##CLS##supertype = BASE::add_subtype(T) ;


  class TYPEDOBJECT_EXPORT TypedObject
  /*--------------------------------*/
  {
   protected:
    virtual void assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,
                                 const rdf::Node &value,  const bool reverse) = 0 ;
    virtual void save_as_rdf(const rdf::Graph &graph) = 0 ;
    virtual bool satisfies_restrictions(const rdf::Graph &graph) ;

   public:
    virtual const rdf::URI &type(void) const = 0 ;
    inline const rdf::URI &uri() const { return m_uri ; }
    static inline int add_subtype(const rdf::URI &T) { return 0 ; }

   public:
    TypedObject() ;
    TypedObject(const std::string &uri) ;
    TypedObject(const std::string &uri, const rdf::Graph &graph) ;

    virtual ~TypedObject() ;

    static TypedObject *create(const rdf::URI &T, const std::string &uri) ;

    template <class T>
    static T *create(std::set<rdf::URI> &subtypes, const rdf::Node &uri, const rdf::Graph &graph)
    /*-----------------------------------------------------------------------------------------*/
    {
      rdf::StatementIter types = graph.get_statements(uri, rdf::RDF::type, rdf::Node()) ;
      if (!types.end()) {
        do {
          rdf::URI type = rdf::URI(types.get_object()) ;
          if (subtypes.find(type) != subtypes.end()) {
            TypedObject *obj = create(type, uri.to_string()) ;
            if (obj->add_metadata(graph)) return dynamic_cast<T *>(obj) ;
            else delete obj ;
            }
          } while (!types.next()) ;
        }
      return nullptr ;
      }


    std::string to_string(void) const ;

    bool operator==(const TypedObject &other) const ;

    bool operator<(const TypedObject &other) const ;

    static void register_type(const rdf::URI &T, TypedObjectFactory *factory) ;

    /**
    Set attributes from RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    bool add_metadata(const rdf::Graph &p_graph) ;

    /**
    Save attributes as RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    void save_metadata(const rdf::Graph &p_graph) ;

    std::string serialise_metadata(const rdf::Graph::Format format=rdf::Graph::Format::RDFXML) ;

   private:
    rdf::URI m_uri ;
    static std::map<rdf::URI, TypedObjectFactory *> &m_factories(void) ;
    } ;

#endif

  } ;

#endif
