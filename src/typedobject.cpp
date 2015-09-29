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


namespace TypedObject {


  TypedObject::TypedObject()
  /*----------------------*/
  : m_uri()
  {
    }

  TypedObject::TypedObject(const rdf::URI &uri)
  /*-----------------------------------------*/
  : m_uri(uri)
  {
    }

  TypedObject::TypedObject(const rdf::URI &uri, const rdf::Graph &graph)
  /*------------------------------------------------------------------*/
  : TypedObject(uri)
  {
    }


  std::map<rdf::URI, TypedObjectFactory *> &TypedObject::m_factories(void)
  /*--------------------------------------------------------------------*/
  {
    static std::map<rdf::URI, TypedObjectFactory *> s_factories ;
    return s_factories ;
    }


  void TypedObject::register_type(const rdf::URI &T, TypedObjectFactory *factory)
  /*---------------------------------------------------------------------------*/
  {
    TypedObject::m_factories()[T] = factory ;
    }


  std::shared_ptr<TypedObject> TypedObject::create(const rdf::URI &T, const std::string &uri)
  /*---------------------------------------------------------------------------------------*/
  {
    return TypedObject::m_factories()[T]->create(uri) ;
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

  bool TypedObject::add_metadata(const rdf::Graph &graph)
  /*---------------------------------------------------*/
  {
    if (m_uri.is_valid()) {
      if (graph.contains(m_uri, rdf::RDF::type, type())               // Needs to be sub-classes
       && satisfies_restrictions(graph)) {
        rdf::StatementIter statements = graph.get_statements(m_uri, rdf::Node(), rdf::Node()) ;
        if (!statements.end()) {
          do {
            assign_from_rdf(graph, statements.get_predicate(), statements.get_object(), false) ;
            } while (!statements.next()) ;
          }
        rdf::StatementIter rstatements = graph.get_statements(rdf::Node(), rdf::Node(), m_uri) ;
        if (!rstatements.end()) {
          do {
            assign_from_rdf(graph, rstatements.get_predicate(), rstatements.get_subject(), true) ;
            } while (!rstatements.next()) ;
          }
        return true ;
        }
      }
    return false ;
    }

  void TypedObject::save_metadata(rdf::Graph &graph)
  /*----------------------------------------------*/
  {
    if (m_uri.is_valid()) {
      graph.insert(m_uri, rdf::RDF::type, type()) ;
      save_as_rdf(graph) ;
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


  } ;
