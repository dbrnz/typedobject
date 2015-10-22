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

#include "typedobject/rdf.h"
#include "rdfimpl.h"
#include "typedobject/typedobject.h"

//**************************************************************************//

#include <string>
#include <set>
#include <limits>

//**************************************************************************//

rdf::Node::Node()
/*-------------*/
: m_node(nullptr)
{
  }

rdf::Node::Node(rdf::NodeImpl * node)
/*---------------------------------*/
: m_node(node)
{
  }

rdf::Node::Node(const rdf::Node & other)
/*------------------------------------*/
: m_node((other.m_node != nullptr) ? new rdf::NodeImpl(*other.m_node) : nullptr)
{
  }

rdf::Node::Node(rdf::Node && other)
/*-------------------------------*/
: m_node(other.m_node)
{
  other.m_node = nullptr;
  }

rdf::Node::~Node()
/*--------------*/
{
  if (m_node != nullptr) delete m_node ;
  }

rdf::Node & rdf::Node::operator=(const rdf::Node & other)
/*-----------------------------------------------------*/
{
  rdf::Node node(other) ;
  *this = std::move(node) ;
  return *this ;
  }

rdf::Node & rdf::Node::operator=(rdf::Node && other)
/*------------------------------------------------*/
{
  std::swap(m_node, other.m_node) ;
  return *this ;
  }  


rdf::NodeImpl *rdf::Node::node(void) const
/*--------------------------------------*/
{
  return m_node ;
  }

bool rdf::Node::operator==(const rdf::Node &other) const
/*----------------------------------------------------*/
{
  return m_node->operator==(*other.m_node) ;
  }

bool rdf::Node::operator<(const rdf::Node &other) const
/*---------------------------------------------------*/
{
  if (m_node == nullptr) return (other.m_node != nullptr) ;
  else                   return m_node->operator<(*other.m_node) ;
  }

rdf::Node::operator std::string() const
/*-----------------------------------*/
{
  return this->to_string() ;
  }

std::ostream & rdf::operator<<(std::ostream & os, const rdf::Node & node)
/*---------------------------------------------------------------------*/
{
  os << node.to_string() ;
  return os ;
  }

bool rdf::Node::is_valid(void) const
/*--------------------------------*/
{
  return (m_node != nullptr)
       && m_node->is_valid()
       && (m_node->to_string() != "") ;
  }

bool rdf::Node::is_literal_type(const char *type_uri) const
/*-------------------------------------------------------*/
{
  return (m_node != nullptr) && m_node->is_literal_type(type_uri) ;
  }

bool rdf::Node::is_uri(void) const
/*--------------------------------*/
{
  return (m_node != nullptr) && m_node->is_uri() ;
  }

bool rdf::Node::is_blank(void) const
/*--------------------------------*/
{
  return (m_node != nullptr) && m_node->is_blank() ;
  }

bool rdf::Node::is_int(void) const
/*------------------------------*/
{
  return (m_node != nullptr) && m_node->is_int() ;
  }

bool rdf::Node::is_float(void) const
/*--------------------------------*/
{
  return (m_node != nullptr) && m_node->is_float() ;
  }

bool rdf::Node::is_bool(void) const
/*-------------------------------*/
{
  return (m_node != nullptr) && m_node->is_bool() ;
  }

std::string rdf::Node::to_string(void) const
/*----------------------------------------*/
{
  return (m_node != nullptr) ? m_node->to_string() : "" ;
  }

int rdf::Node::to_int(void) const
/*-----------------------------*/
{
  return (m_node != nullptr) ? m_node->to_int() : 0 ;
  }

float rdf::Node::to_float(void) const
/*---------------------------------*/
{
  return (m_node != nullptr) ? m_node->to_float() : 0.0 ;
  }

bool rdf::Node::to_bool(void) const 
/*-------------------------------*/
{
  return (m_node != nullptr) ? m_node->to_bool() : false ;
  }


//**************************************************************************//

rdf::URI::URI()
/*-----------*/
: rdf::Node()
{
  }

rdf::URI::URI(const std::string &uri)
/*---------------------------------*/
: rdf::Node(rdf::NodeImpl::new_node(Sord::Node::URI, uri))
{
  }

rdf::URI::URI(const std::string &uri, const std::string &base)
/*----------------------------------------------------------*/
: rdf::Node(rdf::NodeImpl::new_relative_uri(uri, base))
{
  }

rdf::URI::URI(const Node &uri)
/*--------------------------*/
: rdf::Node(rdf::NodeImpl::new_node(Sord::Node::URI, uri.to_string()))
{
  }

rdf::URI::URI(const rdf::URI & other)
/*---------------------------------*/
: rdf::Node(other)
{
  }

rdf::URI::URI(rdf::URI && other)
/*----------------------------*/
: rdf::Node(std::move(other))
{
  }

rdf::URI & rdf::URI::operator=(const rdf::URI & other)
/*--------------------------------------------------*/
{
  return static_cast<URI &>(rdf::Node::operator=(other)) ;
  }

rdf::URI & rdf::URI::operator=(rdf::URI && other)
/*---------------------------------------------*/
{
  return static_cast<URI &>(rdf::Node::operator=(std::move(other))) ;
  }

//**************************************************************************//

rdf::BNode::BNode()
/*---------------*/
: rdf::Node(rdf::NodeImpl::new_node(Sord::Node::BLANK, ""))
{
  }

rdf::BNode::BNode(const std::string &s)
/*-----------------------------------*/
: rdf::Node(rdf::NodeImpl::new_node(Sord::Node::BLANK, s))
{
  }


rdf::BNode::BNode(const rdf::BNode & other)
/*---------------------------------------*/
: rdf::Node(other)
{
  }

rdf::BNode::BNode(rdf::BNode && other)
/*----------------------------------*/
: rdf::Node(std::move(other))
{
  }

rdf::BNode & rdf::BNode::operator=(const rdf::BNode & other)
/*--------------------------------------------------------*/
{
  return static_cast<BNode &>(rdf::Node::operator=(other)) ;
  }

rdf::BNode & rdf::BNode::operator=(rdf::BNode && other)
/*---------------------------------------------------*/
{
  return static_cast<BNode &>(rdf::Node::operator=(std::move(other))) ;
  }


//**************************************************************************//

rdf::Literal::Literal()
/*-------------------*/
: rdf::Node(new rdf::NodeImpl())
{
  }

rdf::Literal::Literal(const std::string &s)
/*---------------------------------------*/
: rdf::Node(rdf::NodeImpl::new_node(Sord::Node::LITERAL, s))
{
  }


rdf::Literal::Literal(const std::string &s, const rdf::URI &datatype)
/*-----------------------------------------------------------------*/
: rdf::Node(rdf::NodeImpl::new_datatype_node(s, datatype))
{
  }

rdf::Literal::Literal(const std::string &s, const std::string &language)
/*--------------------------------------------------------------------*/
: rdf::Node(rdf::NodeImpl::new_language_node(s, language))
{
  }

rdf::Literal::Literal(xsd::Decimal d, unsigned frac_digits)
/*-------------------------------------------------------*/
: rdf::Node(rdf::NodeImpl::new_decimal_node(d, frac_digits))
{
  }

rdf::Literal::Literal(xsd::Integer i)
/*---------------------------------*/
: rdf::Node(rdf::NodeImpl::new_integer_node(i))
{
  }


rdf::Literal::Literal(const rdf::Literal & other)
/*---------------------------------------------*/
: rdf::Node(other)
{
  }

rdf::Literal::Literal(rdf::Literal && other)
/*----------------------------------------*/
: rdf::Node(std::move(other))
{
  }

rdf::Literal & rdf::Literal::operator=(const rdf::Literal & other)
/*--------------------------------------------------------------*/
{
  return static_cast<Literal &>(rdf::Node::operator=(other)) ;
  }

rdf::Literal & rdf::Literal::operator=(rdf::Literal && other)
/*---------------------------------------------------------*/
{
  return static_cast<Literal &>(rdf::Node::operator=(std::move(other))) ;
  }


const char * rdf::Literal::Constants::EMPTY_STRING
/*---------------------------------------------------*/
= "" ;

const xsd::Decimal rdf::Literal::Constants::EMPTY_DECIMAL
/*-----------------------------------------------------*/
 = std::nan("") ;

const xsd::Integer rdf::Literal::Constants::EMPTY_INTEGER
/*-----------------------------------------------------*/
 = std::numeric_limits<xsd::Integer>::lowest() ;


//**************************************************************************//

rdf::Namespace::Namespace(const std::string &name, const std::string &uri)
/*---------------------------------------------------------------------*/
: m_name(name), m_uri(uri)
{
  }

rdf::Namespace::Namespace(const rdf::Namespace & other)
/*---------------------------------------------------*/
: m_name(other.m_name), m_uri(other.m_uri)
{
  }

rdf::Namespace & rdf::Namespace::operator=(const rdf::Namespace & other)
/*--------------------------------------------------------------------*/
{
  (rdf::Literal)m_name = other.m_name ;
  (rdf::URI)m_uri = other.m_uri ;
  return *this ;
  }


bool rdf::Namespace::operator==(const rdf::Namespace &other) const
/*--------------------------------------------------------------*/
{
  return m_uri == other.m_uri ;
  }

bool rdf::Namespace::operator<(const rdf::Namespace &other) const
/*-------------------------------------------------------------*/
{
  return m_uri < other.m_uri ;
  }


const rdf::URI rdf::Namespace::make_URI(const std::string &suffix) const
/*---------------------------------------------------------------------*/
{
  return rdf::URI(m_uri.to_string() + suffix) ;
  }


rdf::Literal rdf::Namespace::name(void) const
/*------------------------------------------*/
{
  return m_name ;
  }

rdf::URI rdf::Namespace::uri(void) const
/*------------------------------------*/
{
  return m_uri ;
  }


//**************************************************************************//

rdf::Statement::Statement()
/*-----------------------*/
: m_stmnt(nullptr)
{
  }

rdf::Statement::Statement(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o)
/*---------------------------------------------------------------------------------*/
: m_stmnt(new rdf::StmntImpl(s.node(), p.node(), o.node()))
{
  }

rdf::Statement::Statement(const rdf::Statement & other)
/*---------------------------------------------------*/
: m_stmnt((other.m_stmnt != nullptr) ? new rdf::StmntImpl(*other.m_stmnt) : nullptr)
{
  }

rdf::Statement::Statement(rdf::Statement && other)
/*----------------------------------------------*/
: m_stmnt(other.m_stmnt)
{
  other.m_stmnt = nullptr ;
  }

rdf::Statement::~Statement()
/*------------------------*/
{
  if (m_stmnt != nullptr) delete m_stmnt ;
  }

rdf::Statement & rdf::Statement::operator=(const rdf::Statement & other)
/*--------------------------------------------------------------------*/
{
  rdf::Statement stmnt(other) ;
  *this = std::move(stmnt) ;
  return *this ;
  }

rdf::Statement & rdf::Statement::operator=(rdf::Statement && other)
/*---------------------------------------------------------------*/
{
  std::swap(m_stmnt, other.m_stmnt) ;
  return *this ;
  }  


//**************************************************************************//

rdf::StatementIter::StatementIter()
/*-------------------------------*/
: m_iter(nullptr)
{
  }

rdf::StatementIter::StatementIter(IterImpl *iter)
/*---------------------------------------------*/
: m_iter(iter)
{
  }

rdf::StatementIter::~StatementIter()
/*--------------------------------*/
{
  if (m_iter != nullptr) delete m_iter ;
  }


bool rdf::StatementIter::end(void) const
/*------------------------------------*/
{
  return m_iter->end() ;
  }

bool rdf::StatementIter::next(void) const
/*-------------------------------------*/
{
  return m_iter->next() ;
  }

rdf::StatementIter & rdf::StatementIter::operator++(void)
/*-----------------------------------------------------*/
{
  m_iter++ ;
  return *this ;
  }

const rdf::Node rdf::StatementIter::get_subject(void) const
/*-------------------------------------------------------*/
{
  return rdf::Node(new rdf::NodeImpl(m_iter->get_subject())) ;
  }

const rdf::Node rdf::StatementIter::get_predicate(void) const
/*---------------------------------------------------------*/
{
  return rdf::Node(new rdf::NodeImpl(m_iter->get_predicate())) ;
  }

const rdf::Node rdf::StatementIter::get_object(void) const
/*------------------------------------------------------*/
{
  return rdf::Node(new rdf::NodeImpl(m_iter->get_object())) ;
  }


//**************************************************************************//

rdf::Graph::Graph()
/*---------------*/
: m_uri(), m_graph(new rdf::GraphImpl()),
  m_objectregistry(std::unordered_map<rdf::URI, std::shared_ptr<tobj::TypedObject>>())
{
  }

rdf::Graph::Graph(const std::string &uri)
/*-------------------------------------*/
: m_uri(uri), m_graph(new rdf::GraphImpl()),
  m_objectregistry(std::unordered_map<rdf::URI, std::shared_ptr<tobj::TypedObject>>())
{
  }

rdf::Graph::~Graph()
/*----------------*/
{
  if (m_graph != nullptr) delete m_graph ;
  }


const std::string rdf::Graph::format_to_mimetype(rdf::Graph::Format format)
/*-----------------------------------------------------------------------*/
{
  switch (format) {
   case rdf::Graph::Format::RDFXML:
    return "application/rdf+xml" ;
   case rdf::Graph::Format::TURTLE:
    return "text/turtle" ;
   case rdf::Graph::Format::NTRIPLES:
    return "application/n-triples" ;
   case rdf::Graph::Format::JSON:
    return "application/json" ;
   default:
    return "application/rdf+xml" ;
    }
  }

rdf::Graph::Format rdf::Graph::mimetype_to_format(const std::string &mimetype)
/*--------------------------------------------------------------------------*/
{
  if      (mimetype == "application/rdf+xml")
    return rdf::Graph::Format::RDFXML ;
  else if (mimetype == "text/turtle")
    return rdf::Graph::Format::TURTLE ;
  else if (mimetype == "application/n-triples")
    return rdf::Graph::Format::NTRIPLES ;
  else if (mimetype == "application/json")
    return rdf::Graph::Format::JSON ;
  else
    return rdf::Graph::Format::RDFXML ;
  }


void rdf::Graph::parse_resource(
/*----------------------------*/
  const std::string &resource, const rdf::Graph::Format format, const std::string &base)
{
  m_graph->parse_resource(resource, format, base) ;
  }

void rdf::Graph::parse_string(
/*--------------------------*/
  const std::string &source, const rdf::Graph::Format format, const std::string &base)
{
  m_graph->parse_string(source, format, base) ;
  }


std::string rdf::Graph::serialise(
/*-------------------------------*/
  const rdf::Graph::Format format, const std::string &base, const std::set<rdf::Namespace> &prefixes)
{
  const uint8_t *base_str = (base != "") ? (const uint8_t*)base.c_str()
                          : m_uri.is_valid() ? m_uri.node()->to_u_string()
                          : nullptr ;
  return m_graph->serialise(format, base_str, prefixes) ;
  }

void rdf::Graph::add_prefixes(const std::set<rdf::Namespace> &prefixes)
/*-------------------------------------------------------------------*/
{
  m_graph->add_prefixes(prefixes) ;
  }

const rdf::URI &rdf::Graph::get_uri(void) const
/*-------------------------------------------*/
{
  return m_uri ;
  }


bool rdf::Graph::insert(const Statement &statement) const
/*-----------------------------------------------------*/
{
  return sord_add(m_graph->model(), *statement.m_stmnt->quad()) ;
  }

bool rdf::Graph::insert(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const
/*-------------------------------------------------------------------------------------*/
{
  SordQuad quad = { s.node()->c_obj(), p.node()->c_obj(), o.node()->c_obj(), NULL } ;
  return sord_add(m_graph->model(), quad) ;
  }


void rdf::Graph::add_statements(const StatementIter &statements) const
/*------------------------------------------------------------------*/
{
  if (!statements.end()) {
    do {
      SordQuad quad ;
      sord_iter_get(statements.m_iter->c_obj(), quad) ;
      sord_add(m_graph->model(), quad) ;
      } while (!statements.next()) ;
    }
  }

bool rdf::Graph::contains(const Statement &statement) const
/*-------------------------------------------------------*/
{
  return sord_contains(m_graph->model(), *statement.m_stmnt->quad()) ;
  }

bool rdf::Graph::contains(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const
/*---------------------------------------------------------------------------------------*/
{
  return contains(rdf::Statement(s, p, o)) ;
  }


rdf::StatementIter rdf::Graph::get_statements(const rdf::Statement &pattern) const
/*------------------------------------------------------------------------------*/
{
  return rdf::StatementIter(new rdf::IterImpl(sord_find(m_graph->model(), *pattern.m_stmnt->quad()))) ;
  }

rdf::StatementIter rdf::Graph::get_statements(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const
/*------------------------------------------------------------------------------------------------------------*/
{
  return get_statements(rdf::Statement(s, p, o)) ;
  }


tobj::TypedObject::Reference rdf::Graph::get_reference(const rdf::URI &uri)
/*-----------------------------------------------------------------------*/
{
  return tobj::TypedObject::get_reference(uri, m_objectregistry) ;
  }

void rdf::Graph::add_reference(const rdf::URI &uri, tobj::TypedObject::Reference reference)
/*---------------------------------------------------------------------------------------*/
{
  tobj::TypedObject::add_reference(uri, reference, m_objectregistry) ;
  }

void rdf::Graph::delete_reference(const rdf::URI &uri)
/*--------------------------------------------------*/
{
  tobj::TypedObject::delete_reference(uri, m_objectregistry) ;
  }


//**************************************************************************//
