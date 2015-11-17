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

#include "typedobject/typedobject.h"


namespace tobj {

  TypedObject::TypedObject()
  /*======================*/
  : m_graph{nullptr}, m_uri{}, m_resources{ResourceMap{}}
  {
    }

  TypedObject::TypedObject(const rdf::URI &uri)
  /*=========================================*/
  : m_graph{nullptr}, m_uri{uri}, m_resources{ResourceMap{}}
  {
    }


  std::unordered_map<rdf::URI, TypedObjectFactory *> &TypedObject::m_factories(void)
  /*------------------------------------------------------------------------------*/
  {
    static std::unordered_map<rdf::URI, TypedObjectFactory *> s_factories ;
    return s_factories ;
    }

  std::set<rdf::URI> &TypedObject::m_subtypes(void)
  /*---------------------------------------------*/
  {
    static std::set<rdf::URI> s_subtypes ;
    return s_subtypes ;
    }

  int TypedObject::add_subtype(const rdf::URI &type)
  /*----------------------------------------------*/
  {
    m_subtypes().insert(type) ;
    return 0 ;
    }

  void TypedObject::register_type(const rdf::URI &T, TypedObjectFactory *factory)
  /*---------------------------------------------------------------------------*/
  {
    TypedObject::m_factories()[T] = factory ;  // For last declared
    }


  TypedObject::Ptr TypedObject::get_resource(const rdf::URI &uri, TypedObject::Registry &registry)
  /*--------------------------------------------------------------------------------------------*/
  {
    auto refptr = registry.find(uri) ;
    if (refptr != registry.end()) {
      TypedObject::Ptr pointer = refptr->second.lock() ;
      if (pointer) return pointer ;
      registry.erase(refptr) ;
      }
    return nullptr ;
    }

  void TypedObject::add_resource(const rdf::URI &uri, TypedObject::WeakPtr weakptr,
  /*-----------------------------------------------------------------------------*/
                                 TypedObject::Registry &registry)
  {
    registry.emplace(uri, weakptr) ;
    }

  void TypedObject::delete_resource(const rdf::URI &uri, TypedObject::Registry &registry)
  /*-----------------------------------------------------------------------------------*/
  {
    registry.erase(uri) ;
    }


  void TypedObject::delete_resource(const rdf::URI &uri)
  /*--------------------------------------------------*/
  {
    m_resources.erase(uri) ;
    }


  bool TypedObject::operator==(const TypedObject &other) const
  /*--------------------------------------------------------*/
  {
    return m_uri.operator==(other.uri()) ;
    }

  bool TypedObject::operator<(const TypedObject &other) const
  /*-------------------------------------------------------*/
  {
    return m_uri.operator<(other.uri()) ;
    }


  bool TypedObject::is_valid(void) const
  /*----------------------------------*/
  {
    return m_uri.is_valid() ;
    }

  std::string TypedObject::to_string(void) const
  /*------------------------------------------*/
  {
    return rdf_type().to_string() + ": <" + m_uri.to_string() + ">" ;
    }

  bool TypedObject::satisfies_restrictions(rdf::Graph::Ptr &graph)
  /*------------------------------------------------------------*/
  {
    return true ;
    }

  rdf::Node TypedObject::get_property(const std::string & name)
  /*---------------------------------------------------------*/
  {
    return rdf::Node() ;
    }


  void TypedObject::save_metadata(rdf::Graph::Ptr &graph)
  /*---------------------------------------------------*/
  {
    if (m_uri.is_valid()) {     // Check if object isn't in graph
      const rdf::Statement hastype(m_uri, rdf::RDF::type, rdf_type()) ;
      if (!graph->contains(hastype)) {
        graph->insert(hastype) ;
        save_as_rdf(graph) ;
        for (auto const &r : m_resources) {
          if (r.second.first) r.second.first->save_metadata(graph) ;
          }
        }
      }
    }

  std::string TypedObject::serialise_metadata(const rdf::Graph::Format format,
  /*------------------------------------------------------------------------*/
                                              const std::string &base, bool append,
                                              const std::set<rdf::Namespace> &prefixes)
  {
    auto graph = rdf::Graph::create(m_uri) ;
    save_metadata(graph) ;
    if (m_graph && append) {
      auto stmnt = graph->get_statements(rdf::Statement(rdf::Node(), rdf::Node(), rdf::Node())) ;
      if (!stmnt.end()) {
        do {
          m_graph->insert(stmnt.get_statement()) ;
          } while (!stmnt.next()) ;
        }
      graph = m_graph ;
      }
    auto md = graph->serialise(format, base, prefixes) ;
    return md ;
    }

  } ;         // End `tobj` namespace
