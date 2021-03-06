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

#ifndef TYPEDOBJECT_RDFIMPL_H
#define TYPEDOBJECT_RDFIMPL_H

#include <sord/sordmm.hpp>
#include <sord/sord.h>
#include <serd/serd.h>

#include <string>
#include <set>


namespace rdf {

  class NodeImpl : public Sord::Node
  /*------------------------------*/
  {
   public:
   	NodeImpl() ;
   	NodeImpl(const SordNode *node) ;
   	NodeImpl(const NodeImpl &other) ;

   	static NodeImpl *new_node(Sord::Node::Type type, const std::string &s) ;
    static NodeImpl *new_relative_uri(const std::string &uri, const std::string &base) ;
    static NodeImpl *new_datatype_node(const std::string &s, const rdf::URI &datatype) ;
    static NodeImpl *new_language_node(const std::string &s, const std::string &language) ;
    static NodeImpl *new_decimal_node(xsd::Decimal d, unsigned frac_digits) ;
    static NodeImpl *new_integer_node(xsd::Integer i) ;
    static NodeImpl *new_node_from_serd_node(const SerdNode *node, const SerdNode *type, const SerdNode *lang) ;
    } ;


  class StmntImpl
  /*-----------*/
  {
   public:
    StmntImpl(const NodeImpl *s, const NodeImpl *p, const NodeImpl *o) ;
   	StmntImpl(const StmntImpl &other) ;
   	StmntImpl(const SordQuad &quad) ;

    const SordQuad *quad(void) const ;

   private:
    SordQuad m_quad ;
    } ;


  class IterImpl : public Sord::Iter
  /*------------------------------*/
  {
   public:
    IterImpl(SordIter *iter) ;

    StmntImpl *get_statement(void) const ;
    const SordNode *get_subject(void) const ;
    const SordNode *get_predicate(void) const ;
    const SordNode *get_object(void) const ;
    } ;


  class GraphImpl
  /*-----------*/
  {
   public:
    GraphImpl() ;
   	GraphImpl(const GraphImpl &other) ;
    virtual ~GraphImpl() ;

    SordModel *model(void) const ;

    void parse_resource(const std::string &resource,
                        const rdf::Graph::Format format=rdf::Graph::Format::RDFXML,
                        const std::string &base="") ;
    void parse_string(const std::string &source,
                      const rdf::Graph::Format format=rdf::Graph::Format::RDFXML,
                      const std::string &base="") ;

    std::string serialise(const rdf::Graph::Format format=rdf::Graph::Format::RDFXML,
                          const uint8_t *base_str=nullptr,
                          const std::set<Namespace> &prefixes=std::set<Namespace>()) ;

    void add_prefixes(const std::set<rdf::Namespace> &prefixes) ;

   private:
    SordModel *m_model ;
    std::set<rdf::Namespace> m_prefixes ;
    } ;

  } ;

#endif
