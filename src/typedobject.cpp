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
  /*----------------------*/
  : m_uri(), m_resources(ResourceMap())
  {
    }

  TypedObject::TypedObject(const rdf::URI &uri)
  /*-----------------------------------------*/
  : m_uri(uri), m_resources(ResourceMap())
  {
    }


  TypedObject::Reference TypedObject::get_reference(const rdf::URI &uri,
  /*------------------------------------------------------------------*/
                                                    TypedObject::Registry &registry)
  {
    auto refptr = registry.find(uri) ;
    if (refptr != registry.end()) {
      TypedObject::Reference reference = refptr->second.lock() ;
      if (reference) return reference ;
      registry.erase(refptr) ;
      }
    return nullptr ;
    }

  void TypedObject::add_reference(const rdf::URI &uri, TypedObject::WeakRef weakref,
  /*------------------------------------------------------------------------------*/
                                  TypedObject::Registry &registry)
  {
    registry.emplace(uri, weakref) ;
    }

  void TypedObject::delete_reference(const rdf::URI &uri, TypedObject::Registry &registry)
  /*------------------------------------------------------------------------------------*/
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
    return type().to_string() + ": <" + m_uri.to_string() + ">" ;
    }

  bool TypedObject::satisfies_restrictions(const rdf::Graph &graph)
  /*-------------------------------------------------------------*/
  {
    return true ;
    }

  rdf::Node TypedObject::get_property(const std::string & name)
  /*---------------------------------------------------------*/
  {
    return rdf::Node() ;
    }


  void TypedObject::save_metadata(rdf::Graph &graph)
  /*----------------------------------------------*/
  {
    if (m_uri.is_valid()) {     // Check if object isn't in graph
      const rdf::Statement hastype(m_uri, rdf::RDF::type, type()) ;
      if (!graph.contains(hastype)) {
        graph.insert(hastype) ;
        save_as_rdf(graph) ;
        for (auto const &r : m_resources) {
          if (r.second.first) r.second.first->save_metadata(graph) ;
          }
        }
      }
    }

  std::string TypedObject::serialise_metadata(
  /*-----------------------------------------*/
    const rdf::Graph::Format format, const std::string &base, const std::set<rdf::Namespace> &prefixes)
  {
    rdf::Graph g ;
    save_metadata(g) ;
    return g.serialise(format, base, prefixes) ;
    }

  } ;         // End `tobj` namespace
