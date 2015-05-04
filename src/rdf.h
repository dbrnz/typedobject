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

#ifndef TYPEDOBJECT_RDF_H
#define TYPEDOBJECT_RDF_H

#include "typedobject_export.h"

//**************************************************************************//

#include <string>
#include <list>
#include <cmath>

//**************************************************************************//

namespace rdf {

  class NodeImpl ;                   // Declare forward
  class StmntImpl ;                  // Declare forward
  class IterImpl ;                   // Declare forward
  class GraphImpl ;                  // Declare forward

  class Graph ;                      // Declare forward for VS2013


  typedef int64_t Integer ;
  /*---------------------*/

  typedef double Decimal ;
  /*--------------------*/


  class TYPEDOBJECT_EXPORT Node
  /*-------------------------*/
  {
   public:
    Node() ;
    Node(NodeImpl * node) ;
    Node(const Node & other) ;
    Node(Node && other) noexcept ;
    virtual ~Node() noexcept ;
    Node & operator=(const Node & other) ;
    Node & operator=(Node && other) noexcept ;

    NodeImpl *node(void) const ;

    bool operator==(const Node &other) const ;
    bool operator<(const Node &other) const ;

    bool is_valid(void) const ;
    bool is_literal_type(const char *type_uri) const ;
    bool is_uri(void) const ;
    bool is_blank(void) const ;
    bool is_int(void) const ;
    bool is_float(void) const ;
    bool is_bool(void) const ;

    std::string to_string(void) const ;
    int to_int(void) const ;
    float to_float(void) const ;
    bool to_bool(void) const  ;

   private:
    NodeImpl *m_node ;
    } ;


  class TYPEDOBJECT_EXPORT URI : public Node
  /*--------------------------------------*/
  {
   public:
    URI() ;
    URI(const std::string & uri) ;
    URI(const Node & uri) ;
    URI(const URI & other) ;
    URI(URI && other) noexcept ;
    URI & operator=(const URI & other) ;
    URI & operator=(URI && other) noexcept ;
    } ;


  class TYPEDOBJECT_EXPORT BNode : public Node
  /*----------------------------------------*/
  {
   public:
    BNode() ;
    BNode(const std::string & s) ;
    BNode(const BNode & other) ;
    BNode(BNode && other) noexcept ;
    BNode & operator=(const BNode & other) ;
    BNode & operator=(BNode && other) noexcept ;
    } ;


  class TYPEDOBJECT_EXPORT Literal : public Node
  /*------------------------------------------*/
  {
   public:
    Literal() ;
    Literal(const std::string & s) ;
    Literal(const std::string & s, const rdf::URI & datatype) ;
    Literal(const std::string & s, const std::string & language) ;
    Literal(Decimal d, unsigned frac_digits) ;
    Literal(Integer i) ;
    Literal(const Literal & other) ;
    Literal(Literal && other) noexcept ;
    Literal & operator=(const Literal & other) ;
    Literal & operator=(Literal && other) noexcept ;

    class Constants {
     public:
      static const std::string EMPTY_STRING ;
      static const rdf::Decimal EMPTY_DECIMAL ;
      static const rdf::Integer EMPTY_INTEGER ;
      } ;
    inline static bool not_empty(const std::string &s) { return (s != Constants::EMPTY_STRING) ; }
    inline static bool not_empty(const Decimal d) { return std::isfinite(d) ; }
    inline static bool not_empty(const Integer i) { return (i != Constants::EMPTY_INTEGER) ; }
    } ;


  class TYPEDOBJECT_EXPORT Namespace
  /*------------------------------*/
  {
   public:
    Namespace(const std::string & name, const std::string & uri) ;

    const URI make_URI(const std::string &suffix) const ;

    Literal name(void) const ;
    URI uri(void) const ;

   private:
    const Literal m_name ;
    const URI m_uri ;
    } ;


  class TYPEDOBJECT_EXPORT Statement
  /*------------------------------*/
  {
   public:
    Statement() ;
    Statement(const Node & s, const Node & p, const Node & o) ;
    Statement(const Statement & other) ;
    Statement(Statement && other) noexcept ;
    virtual ~Statement() noexcept ;
    Statement & operator=(const Statement & other) ;
    Statement & operator=(Statement && other) noexcept ;

   private:
    StmntImpl *m_stmnt ;
    friend class Graph ;
    } ;


  class TYPEDOBJECT_EXPORT StatementIter
  /*----------------------------------*/
  {
   public:
    StatementIter() ;
    StatementIter(IterImpl * iter) ;
    virtual ~StatementIter() noexcept ;

    bool end(void) const ;
    bool next(void) const ;
    StatementIter & operator++ (void) ;
    const Node get_subject(void) const ;
    const Node get_predicate(void) const ;
    const Node get_object(void) const ;
    const Statement get_statement(void) const ;

   private:
    IterImpl *m_iter ;
    friend class Graph ;
    } ;


  class TYPEDOBJECT_EXPORT Graph
  /*--------------------------*/
  {
   public:
    Graph() ;
    Graph(const std::string & uri) ;
    virtual ~Graph() noexcept ;

    enum class Format {
      UNKNOWN = 0,
      RDFXML,
      TURTLE,
      NTRIPLES
      } ;

    void parse_resource(const std::string &resource,
                        const Format format=Format::RDFXML,
                        const std::string &base="") ;
    void parse_string(const std::string &source,
                      const Format format=Format::RDFXML,
                      const std::string &base="") ;

    std::string serialise(const Format format=Format::RDFXML,
                          const std::string &base="",
                          const std::list<Namespace> &prefixes=std::list<Namespace>()) ;

    const URI &get_uri(void) const ;

    const bool insert(const Statement &statement) const ;
    const bool insert(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const ;

    void add_statements(const StatementIter &statements) const ;

    const bool contains(const Statement &p_statement) const ;
    const bool contains(const Node &s, const Node &p, const Node &o) const ;

    StatementIter get_statements(const Statement &pattern) const ;
    StatementIter get_statements(const Node &s, const Node &p, const Node &o) const ;

   private:
    const URI m_uri ;
    GraphImpl *m_graph ;
    } ;


  } ;

//**************************************************************************//

#endif
