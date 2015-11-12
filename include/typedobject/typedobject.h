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
#include <typedobject/common.h>
#include <typedobject/rdf.h>
#include <typedobject/rdfdefs.h>
#include <typedobject/xsd.h>

#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <set>
#include <list>
#include <typeinfo>
#include <typeindex>


#ifdef TYPED_OBJECT_COMPILE

#define REFERENCE(CLASS)

int _PARAMETERS_(const char *params, ...) { return 0 ; }

#define _FORWARD_OBJECT(CLASS, TYPE)      \
  static int _FORWARD_##CLASS##  = _PARAMETERS_("1", #TYPE) ;

#define _TYPED_OBJECT(CLASS, TYPE)        \
  static int _OBJECT_##CLASS##   = _PARAMETERS_("1", #TYPE) ;

#define _PROPERTY(NAME, P, T, ...)        \
  static int _PROPERTY_##NAME##  = _PARAMETERS_("2", #T, #P, #__VA_ARGS__) ;

#define _PROPERTY_OBJ(NAME, P, T, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_("3", #T, #P, "OBJ",  #__VA_ARGS__) ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
  static int _PROPERTY_##NAME##  = _PARAMETERS_("3", #T, #P, "SET",  #__VA_ARGS__) ;

#define _PROPERTY_OBJ_SET(NAME, P, T, ...)\
  static int _PROPERTY_##NAME##  = _PARAMETERS_("4", #T, #P, "SET",  "OBJ", #__VA_ARGS__) ;

#define _ASSIGN(NAME, P, T, ...)          \
  static int _ASSIGN_##NAME##    = _PARAMETERS_("2", #T, #P, #__VA_ARGS__) ;

#define _RESTRICTION(NAME, VALUE, ...)    \
  static int _RESTRICT_##NAME##  = _PARAMETERS_("1", #VALUE, #__VA_ARGS__) ;

#define _INITIALISE(CODE, ...)            \
  static int _INITIALISE_        = _PARAMETERS_("1", #CODE, #__VA_ARGS__) ;

#define _PREFIXES(LIST, ...)              \
  static int _PREFIXES_          = _PARAMETERS_("1", #LIST, #__VA_ARGS__) ;

#define _RESOURCE(P, T, ...)              \
  static int _RESOURCE_##T##     = _PARAMETERS_("1", #P, #__VA_ARGS__) ;

#else

#define _FORWARD_OBJECT(CLASS, TYPE)                                        \
  class CLASS ;

#define _TYPED_OBJECT(CLASS, TYPE)                                          \
 public:                                                                    \
  CLASS() = default ;                                                       \
  CLASS(const rdf::URI &uri) ;                                              \
  static const rdf::URI rdf_type ;                                          \
  const rdf::URI &type(void) const override ;                               \
  static std::set<rdf::URI> &subtypes(void) ;                               \
  static int add_subtype(const rdf::URI &T) ;                               \
  SHARED_PTR(CLASS)                                                         \
  void add_prefix(const rdf::Namespace &prefix) ;                           \
 protected:                                                                 \
  bool satisfies_restrictions(rdf::Graph::Ptr &graph) override ;            \
  static rdf::Node get_property(const std::string &name) ;                  \
  void assign_from_rdf(rdf::Graph::Ptr &graph, const rdf::Node &property,   \
               const rdf::Node &value, const bool reverse) override ;       \
  void save_as_rdf(rdf::Graph::Ptr &graph) override ;                       \
 private:                                                                   \
  static std::unordered_map<std::string, rdf::Node> s_properties ;          \
  static const std::set<rdf::Namespace> s_prefixes ;                        \
  std::set<rdf::Namespace> m_prefixes ;

#define _PROPERTY(NAME, P, T, ...)        \
 public:                                  \
  inline const T & NAME(void) const       \
    { return p_##NAME ; }                 \
  inline void set_##NAME(const T & value) \
    { p_##NAME = value ; }                \
 protected:                               \
  T p_##NAME ;

#define _PROPERTY_OBJ(NAME, P, T, ...)    \
 public:                                  \
  inline const std::shared_ptr<T> NAME(void) const \
    { return p_##NAME ; }                 \
  inline void set_##NAME(std::shared_ptr<T> value) \
    { p_##NAME = value ; }                \
 private:                                 \
  std::shared_ptr<T> p_##NAME ;

#define _PROPERTY_SET(NAME, P, T, ...)    \
 public:                                  \
  inline const std::set<T> & NAME(void) const \
    { return p_##NAME ; }                 \
  inline void add_##NAME(const T & value) \
    { p_##NAME.insert(value) ; }          \
 private:                                 \
  std::set<T> p_##NAME ;

#define _PROPERTY_OBJ_SET(NAME, P, T, ...)\
 public:                                  \
  inline const std::set<T::Ptr> & NAME(void) const \
    { return p_##NAME ; }                 \
  inline void add_##NAME(T::Ptr value)    \
    { p_##NAME.insert(value) ; }          \
 private:                                 \
  std::set<T::Ptr> p_##NAME ;


#define _ASSIGN(NAME, P, T, ...)
#define _RESTRICTION(NAME, VALUE, T, ...)

#define _INITIALISE(CODE, ...)
#define _PREFIXES(CODE, ...)

#define _RESOURCE(P, T)

#endif

#define TYPED_OBJECT(CLASS, TYPE)        _TYPED_OBJECT(CLASS, TYPE)
#define FORWARD_OBJECT(CLASS, TYPE)      _FORWARD_OBJECT(CLASS, TYPE)

#define PROPERTY_STRING(NAME, P)         _PROPERTY(NAME, P, std::string)
#define PROPERTY_INTEGER(NAME, P)        _PROPERTY(NAME, P, xsd::Integer)
#define PROPERTY_DECIMAL(NAME, P)        _PROPERTY(NAME, P, xsd::Decimal)
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

#define ASSIGN_DATETIME(NAME, P)         _ASSIGN(NAME, P, xsd::Datetime)
#define ASSIGN_DURATION(NAME, P)         _ASSIGN(NAME, P, xsd::Duration)

#define RESTRICT_STRING(NAME, VALUE)     _RESTRICTION(NAME, VALUE, std::string)
#define RESTRICT_INTEGER(NAME, VALUE)    _RESTRICTION(NAME, VALUE, xsd::Integer)
#define RESTRICT_DECIMAL(NAME, VALUE)    _RESTRICTION(NAME, VALUE, xsd::Decimal)
#define RESTRICT_NODE(NAME, VALUE)       _RESTRICTION(NAME, VALUE, rdf::Node)
#define RESTRICT_URI(NAME, VALUE)        _RESTRICTION(NAME, VALUE, rdf::URI)

#define INITIALISE(CODE, ...)            _INITIALISE(CODE, #__VA_ARGS__)
#define PREFIXES(LIST, ...)              _PREFIXES(LIST, #__VA_ARGS__)

#define RESOURCE(P, T)                   _RESOURCE(P, T)


namespace tobj
/*==========*/
{

  const std::string VERSION = "0.9.5" ;

#ifdef TYPED_OBJECT_COMPILE

  class TypedObject { } ;

#else

#define REGISTER_TYPES(T, CLS, BASE)                            \
  static int _global_##CLS##_type = CLS::add_subtype(T) ;       \
  static int _global_##CLS##_supertype = BASE::add_subtype(T) ;


  class TYPEDOBJECT_EXPORT TypedObject
  /*--------------------------------*/
  {
   public:
    TypedObject() ;
    TypedObject(const rdf::URI &uri) ;
    virtual ~TypedObject() = default ;

    using Ptr = std::shared_ptr<TypedObject> ;
    using WeakPtr = std::weak_ptr<TypedObject> ;
    using Registry = std::unordered_map<rdf::URI, WeakPtr> ;
    using ResourceInfo = std::pair<Ptr, std::type_index> ;
    using ResourceMap = std::map<rdf::URI, ResourceInfo> ;

    template <class T>
    static typename T::Ptr create_from_graph(const rdf::URI &uri, rdf::Graph::Ptr &graph) ;

    bool operator==(const TypedObject &other) const ;
    bool operator<(const TypedObject &other) const ;

    inline const rdf::URI &uri() const { return m_uri ; }
    inline void set_uri(const rdf::URI &uri) { m_uri = uri ; }
    bool is_valid(void) const ;
    std::string to_string(void) const ;

    virtual const rdf::URI &type(void) const = 0 ;
    static inline int add_subtype(const rdf::URI &T) { (void)T ; return 0 ; } // Unused parameter

    /**
    Set attributes from RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    template <typename T> bool add_metadata(rdf::Graph::Ptr &graph) ;

    /**
    Save attributes as RDF triples in a graph.

    :param graph: A graph of RDF statements.
    :type graph: :class:`~biosignalml.rdf.Graph`
    **/
    void save_metadata(rdf::Graph::Ptr &graph) ;

    std::string serialise_metadata(const rdf::Graph::Format format=rdf::Graph::Format::RDFXML,
                                   const std::string &base="",
                                   const std::set<rdf::Namespace> &prefixes=std::set<rdf::Namespace>()) ;

    template<class T> void add_resource(typename T::Ptr resource) ;
    template<class T> typename T::Ptr get_resource(const rdf::URI &uri) ;
    void delete_resource(const rdf::URI &uri) ;

    template<class T> std::list<rdf::URI> get_resource_uris(void) ;

    template <typename T> bool assign_metadata(rdf::Graph::Ptr &graph) ;

   protected:
    friend class rdf::Graph ;

    virtual void assign_from_rdf(rdf::Graph::Ptr &graph, const rdf::Node &property,
                                 const rdf::Node &value, const bool reverse) = 0 ;
    static rdf::Node get_property(const std::string &name) ;
    virtual void save_as_rdf(rdf::Graph::Ptr &graph) = 0 ;
    virtual bool satisfies_restrictions(rdf::Graph::Ptr &graph) ;

    static Ptr get_resource(const rdf::URI &uri, Registry &registry) ;
    static void add_resource(const rdf::URI &uri, WeakPtr weakptr, Registry &registry) ;
    static void delete_resource(const rdf::URI &uri, Registry &registry) ;

   private:
    rdf::URI m_uri ;
    ResourceMap m_resources ;
    } ;


  template <typename T>
  bool TypedObject::add_metadata(rdf::Graph::Ptr &graph)
  /*--------------------------------------------------*/
  {
    if (m_uri.is_valid()
     && graph.contains(m_uri, rdf::RDF::type, type()))   // Needs to be sub-classes
      return assign_metadata<T>(graph) ;
    return false ;
    }

  template <typename T>
  bool TypedObject::assign_metadata(rdf::Graph::Ptr &graph)
  /*-----------------------------------------------------*/
  {
    if (!satisfies_restrictions(graph)) return false ;
    rdf::StatementIter statements = graph.get_statements(m_uri, rdf::Node(), rdf::Node()) ;
    if (!statements.end()) {
      do {
        this->assign_from_rdf(graph, statements.get_predicate(), statements.get_object(), false) ;
        } while (!statements.next()) ;
      }
    rdf::StatementIter rstatements = graph.get_statements(rdf::Node(), rdf::Node(), m_uri) ;
    if (!rstatements.end()) {
      do {
        this->assign_from_rdf(graph, rstatements.get_predicate(), rstatements.get_subject(), true) ;
        } while (!rstatements.next()) ;
      }
    return true ;
    }

  template <class T>
  typename T::Ptr TypedObject::create_from_graph(const rdf::URI &uri, rdf::Graph::Ptr &graph)
  /*---------------------------------------------------------------------------------------*/
  {
    rdf::StatementIter types = graph.get_statements(uri, rdf::RDF::type, rdf::Node()) ;
    if (!types.end()) {
      do {
        rdf::URI type = rdf::URI(types.get_object()) ;
        if (T::subtypes().find(type) != T::subtypes().end()) {
          auto object = graph.get_resource(uri) ;
          if (object)
            return std::dynamic_pointer_cast<T>(object) ;
          else if (type == T::rdf_type) {
            auto object = std::make_shared<T>((std::string)uri) ;
            graph.add_resource(uri, object) ;
            if (object->template assign_metadata<T>(graph))
              return object ;
            graph.delete_resource(uri) ;
            object.reset() ;
            }
          }
        } while (!types.next()) ;
      }
    return typename T::Ptr() ;
    }


  template<class T>
  void TypedObject::add_resource(typename T::Ptr resource)
  /*----------------------------------------------------*/
  {
    if (resource && resource->is_valid())
      m_resources.emplace(resource->uri(), ResourceInfo(resource, std::type_index(typeid(T)))) ;
    }

  template<class T>
  typename T::Ptr TypedObject::get_resource(const rdf::URI &uri)
  /*----------------------------------------------------------*/
  {
    auto ref = m_resources.find(uri) ;
    if (ref != m_resources.end()
     && std::type_index(typeid(T)) == ref->second.second)
      return std::static_pointer_cast<T>(ref->second.first) ;
    return nullptr ;
    }

  template<class T>
  std::list<rdf::URI> TypedObject::get_resource_uris(void)
  /*----------------------------------------------------*/
  {
    std::list<rdf::URI> uris ;
    for (const auto ref : m_resources)
      if (std::type_index(typeid(T)) == ref.second.second)
        uris.push_back(ref.first) ;
    return uris ;
    }

#endif

  } ;

#endif
