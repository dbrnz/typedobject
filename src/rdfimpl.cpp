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

//**************************************************************************//

#include <raptor2.h>

#include <string>


//**************************************************************************//

static Sord::World &sordWorld()
/*---------------------------*/
{
  static Sord::World instance ;
  return instance;
  }


static raptor_world *raptorWorld(void)
/*----------------------------------*/
{
  static raptor_world *world = NULL ;
  if (world == NULL) {
    world = raptor_new_world() ;
    raptor_world_open(world) ;
    }
  return world ;
  }

//**************************************************************************//

rdf::NodeImpl::NodeImpl()
/*---------------------*/
: Sord::Node(sordWorld())
{
  }

rdf::NodeImpl::NodeImpl(Sord::Node::Type t, const std::string &s)
/*-------------------------------------------------------------*/
: Sord::Node(sordWorld(), t, s)
{
  }

rdf::NodeImpl::NodeImpl(const SordNode *node)
/*-----------------------------------------*/
: Sord::Node(sordWorld(), node)
{
  }

rdf::NodeImpl::NodeImpl(SordNode *node, bool copy)
/*----------------------------------------------*/
: Sord::Node(sordWorld(), node, copy)
{
  }

rdf::NodeImpl::NodeImpl(const NodeImpl &other)
/*------------------------------------------*/
: Sord::Node(other)
{
  }

rdf::NodeImpl *rdf::NodeImpl::new_datatype_node(const std::string &s, const rdf::URI &datatype)
/*-------------------------------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_from_string(SERD_LITERAL, (const uint8_t*)s.c_str()) ;
  const SerdNode *type = sord_node_to_serd_node(datatype.node()->c_obj()) ;
  return rdf::NodeImpl::new_node_from_serd_node(&val, type, NULL) ;
  }

rdf::NodeImpl *rdf::NodeImpl::new_language_node(const std::string &s, const std::string &language)
/*----------------------------------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_from_string(SERD_LITERAL, (const uint8_t*)s.c_str()) ;
  const SerdNode lang = serd_node_from_string(SERD_LITERAL, (const uint8_t*)language.c_str()) ;
  return rdf::NodeImpl::new_node_from_serd_node(&val, NULL, &lang) ;
  }

rdf::NodeImpl *rdf::NodeImpl::new_decimal_node(xsd::Decimal d, unsigned frac_digits)
/*--------------------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_new_decimal(d, frac_digits) ;
  const SerdNode type = serd_node_from_string(SERD_URI, (const uint8_t*)SORD_NS_XSD "decimal") ;
  return rdf::NodeImpl::new_node_from_serd_node(&val, &type, NULL) ;
  }

rdf::NodeImpl *rdf::NodeImpl::new_integer_node(xsd::Integer i)
/*----------------------------------------------------------*/
{
  const SerdNode val = serd_node_new_integer(i) ;
  const SerdNode type = serd_node_from_string(SERD_URI, (const uint8_t*)SORD_NS_XSD "integer") ;
  return rdf::NodeImpl::new_node_from_serd_node(&val, &type, NULL) ;
  }

rdf::NodeImpl *rdf::NodeImpl::new_node_from_serd_node(
/*--------------------------------------------------*/
  const SerdNode *node, const SerdNode *type, const SerdNode *lang)
{
  return
    new rdf::NodeImpl(::sord_node_from_serd_node(sordWorld().c_obj(), sordWorld().prefixes().c_obj(), node, type, lang)) ;
  }

//**************************************************************************//

rdf::StmntImpl::StmntImpl(const rdf::NodeImpl *s, const rdf::NodeImpl *p, const rdf::NodeImpl *o)
/*---------------------------------------------------------------------------------------------*/
{
  m_quad[SORD_SUBJECT]   = (s == nullptr) ? nullptr : s->c_obj() ;
  m_quad[SORD_PREDICATE] = (p == nullptr) ? nullptr : p->c_obj() ;
  m_quad[SORD_OBJECT]    = (o == nullptr) ? nullptr : o->c_obj() ;
  m_quad[SORD_GRAPH]     = nullptr ;
  }

rdf::StmntImpl::StmntImpl(const StmntImpl &other)
/*---------------------------------------------*/
{
  m_quad[SORD_SUBJECT] = sord_node_copy(other.m_quad[SORD_SUBJECT]) ;
  m_quad[SORD_PREDICATE] = sord_node_copy(other.m_quad[SORD_PREDICATE]) ;
  m_quad[SORD_OBJECT] = sord_node_copy(other.m_quad[SORD_OBJECT]) ;
  m_quad[SORD_GRAPH] = nullptr ;
  }

const SordQuad *rdf::StmntImpl::quad(void) const
/*--------------------------------------------*/
{
  return &m_quad ;
  }

//**************************************************************************//

rdf::IterImpl::IterImpl(SordIter *iter)
/*-----------------------------------*/
: Sord::Iter(sordWorld(), iter)
{
  }

const SordNode *rdf::IterImpl::get_subject(void) const
/*--------------------------------------------------*/
{
  SordQuad quad ;
  sord_iter_get(this->c_obj(), quad) ;
  return quad[SORD_SUBJECT] ;
  }

const SordNode *rdf::IterImpl::get_predicate(void) const
/*----------------------------------------------------*/
{
  SordQuad quad ;
  sord_iter_get(this->c_obj(), quad) ;
  return quad[SORD_PREDICATE] ;
  }

const SordNode *rdf::IterImpl::get_object(void) const
/*-------------------------------------------------*/
{
  SordQuad quad ;
  sord_iter_get(this->c_obj(), quad) ;
  return quad[SORD_OBJECT] ;
  }

//**************************************************************************//

rdf::GraphImpl::GraphImpl()
/*-----------------------*/
: m_model(sord_new(sordWorld().c_obj(), SORD_SPO | SORD_OPS, false))
{
  }

rdf::GraphImpl::~GraphImpl()
/*------------------------*/
{
  sord_free(m_model) ;
  }

SordModel *rdf::GraphImpl::model(void) const
/*----------------------------------------*/
{
  return m_model ;
  }


static char *get_raptor_format(const rdf::Graph::Format format)
/*-----------------------------------------------------------*/
{
  static char *format_names[] = {  // Needs to match Graph::Format order
    (char *)"guess",
    (char *)"rdfxml",
    (char *)"turtle",
    (char *)"ntriples"
    } ;
  return format_names[(int)format] ;
  }


static SordNode *sord_node_from_raptor_term(const raptor_term *term)
/*----------------------------------------------------------------*/
{
  SordNode *node = NULL ;
  switch (term->type) {
   case RAPTOR_TERM_TYPE_URI: {
    raptor_uri *uri = term->value.uri ;
    node = sord_new_uri(sordWorld().c_obj(), raptor_uri_as_string(uri)) ;
    }
    break ;
	 case RAPTOR_TERM_TYPE_BLANK: {
    raptor_term_blank_value blank = term->value.blank ;
    node = sord_new_blank(sordWorld().c_obj(), blank.string) ;
    }
    break ;
	 case RAPTOR_TERM_TYPE_LITERAL: {
    raptor_term_literal_value literal = term->value.literal ;
    SordNode *datatype = NULL ;
    if (literal.datatype != NULL)
      datatype = sord_new_uri(sordWorld().c_obj(), raptor_uri_as_string(literal.datatype)) ;
    node = sord_new_literal(sordWorld().c_obj(), datatype, literal.string, (const char *)literal.language) ;
    }
   default:
    break ;
    }
  return node ;
  }

static void process_raptor_message(void *user_data, raptor_log_message *message)
/*----------------------------------------------------------------------------*/
{
  printf("Message: %s\n", message->text) ;
  }


static void process_raptor_namespace(void *user_data, raptor_namespace *nspace)
/*---------------------------------------------------------------------------*/
{
  serd_env_set_prefix_from_strings(sordWorld().prefixes().c_obj(),
                                   raptor_namespace_get_prefix(nspace),
                                   raptor_uri_as_string(raptor_namespace_get_uri(nspace))) ;
  }

static void process_raptor_statement(void *user_data, raptor_statement *triple)
/*---------------------------------------------------------------------------*/
{
  SordQuad quad = { sord_node_from_raptor_term(triple->subject),
                    sord_node_from_raptor_term(triple->predicate),
                    sord_node_from_raptor_term(triple->object),
                    NULL
                  } ;
  sord_add((SordModel *)user_data, quad) ;
  }

static uint8_t *get_file_from_uri_string(const uint8_t *s)
/*------------------------------------------------------*/
{
  uint8_t *path = serd_file_uri_parse((const uint8_t*)s, NULL) ;
  if (path != NULL && serd_uri_string_has_scheme(path)) {
    free(path) ;
    path = NULL ;
    }
  return path ;
  }


static raptor_parser *setup_raptor_parser(
/*--------------------------------------*/
  SordModel *model, const rdf::Graph::Format format, const std::string &base, raptor_uri **base_uri)
{
  raptor_world_set_log_handler(raptorWorld(), nullptr, process_raptor_message) ;
  raptor_parser *parser = raptor_new_parser(raptorWorld(), get_raptor_format(format)) ;
  raptor_parser_set_statement_handler(parser, model, process_raptor_statement) ;
  raptor_parser_set_namespace_handler(parser, nullptr, process_raptor_namespace) ;
  if (base != "") *base_uri = raptor_new_uri(raptorWorld(), (uint8_t *)base.c_str()) ;
  return parser ;
  }


void rdf::GraphImpl::parse_resource(
/*--------------------------------*/
  const std::string &resource, const rdf::Graph::Format format, const std::string &base)
{
  uint8_t *file_path = get_file_from_uri_string((const uint8_t*)resource.c_str()) ;

  if ((format != rdf::Graph::Format::TURTLE && format != rdf::Graph::Format::NTRIPLES)
   || file_path == NULL) {  // Use Raptor...
    std::string base_str = (base != "") ? base : resource ;
    raptor_uri *base_uri = NULL ;
    raptor_parser *parser = setup_raptor_parser(m_model, format, base_str, &base_uri) ;
    raptor_uri *uri = raptor_new_uri(raptorWorld(), (uint8_t *)resource.c_str()) ;
    if (file_path != NULL) {
      FILE *stream = fopen((const char *)file_path, "rb") ;
      int i = raptor_parser_parse_file_stream(parser, stream, (const char *)file_path, base_uri) ;
      fclose(stream) ;
      }
    else raptor_parser_parse_uri(parser, uri, base_uri) ;
    raptor_free_uri(uri) ;
    if (base_uri != NULL) raptor_free_uri(base_uri) ;
    raptor_free_parser(parser) ;
    }
  else {
    SerdReader *reader = sord_new_reader(m_model, sordWorld().prefixes().c_obj(),
                                         (format == rdf::Graph::Format::TURTLE) ? SERD_TURTLE
                                                                                : SERD_NTRIPLES,
                                         NULL) ;
    serd_reader_read_file(reader, file_path) ;
    serd_reader_free(reader) ;
    }
  if (file_path != NULL) free(file_path) ;
  }

void rdf::GraphImpl::parse_string(
/*------------------------------*/
  const std::string &source, const rdf::Graph::Format format, const std::string &base)
{
  if (format == rdf::Graph::Format::TURTLE || format == rdf::Graph::Format::NTRIPLES) {  // Use sord
    SerdReader *reader = sord_new_reader(m_model, sordWorld().prefixes().c_obj(),
                                         (format == rdf::Graph::Format::TURTLE) ? SERD_TURTLE
                                                                                : SERD_NTRIPLES,
                                         NULL) ;
    serd_reader_read_string(reader, (const uint8_t*)source.c_str()) ;
    serd_reader_free(reader) ;
    }
  else {                                                         // Use Raptor
    raptor_uri *base_uri = NULL ;
    raptor_parser *parser = setup_raptor_parser(m_model, format, base, &base_uri) ;
    raptor_parser_parse_start(parser, base_uri) ;
    raptor_parser_parse_chunk(parser, (uint8_t *)source.c_str(), source.length(), 1) ;
    if (base_uri != NULL) raptor_free_uri(base_uri) ;
    raptor_free_parser(parser) ;
    }
  }


static SerdStatus set_raptor_prefix(SerdWriter *writer, const SerdNode *name, const SerdNode *uri)
/*----------------------------------------------------------------------------------------------*/
{
  raptor_serializer *serialiser = (raptor_serializer *)writer ;
  raptor_uri *prefix_uri = raptor_new_uri(raptorWorld(), uri->buf) ;
  raptor_serializer_set_namespace(serialiser, prefix_uri, name->buf) ;
  raptor_free_uri(prefix_uri) ;
  return SERD_SUCCESS ;
  }

static raptor_term *raptor_term_from_sord_node(const SordNode *node)
/*----------------------------------------------------------------*/
{
  const uint8_t *text = sord_node_get_string(node) ;
  raptor_term *term = NULL ;

  switch (sord_node_get_type(node)) {
   case SORD_URI:
    term = raptor_new_term_from_uri_string(raptorWorld(), text) ;
    break ;

   case SORD_BLANK:
    term = raptor_new_term_from_blank(raptorWorld(), text) ;
    break ;

   case SORD_LITERAL: {
    SordNode *dtype = sord_node_get_datatype(node) ;
    raptor_uri *datatype = NULL ;
    if (dtype != NULL)
      datatype = raptor_new_uri(raptorWorld(), sord_node_get_string(dtype)) ;
    const char *language = sord_node_get_language(node) ;
    term = raptor_new_term_from_literal(raptorWorld(), text, datatype, (uint8_t *)language) ;
    if (datatype != NULL) raptor_free_uri(datatype) ;
    }

   default:
    break ;
    }
  return term ;
  }

std::string rdf::GraphImpl::serialise(const rdf::Graph::Format format, const uint8_t *base_str,
/*-------------------------------------------------------------------------------------------*/
                                      const std::list<rdf::Namespace> &prefixes)
{
  std::string result = "" ;
  if (format == rdf::Graph::Format::TURTLE || format == rdf::Graph::Format::NTRIPLES) {     // Use sord
    SerdURI base_uri = SERD_URI_NULL ;
    if (base_str) serd_uri_parse(base_str, &base_uri) ;

    SerdWriter *writer = serd_writer_new((format == rdf::Graph::Format::TURTLE) ? SERD_TURTLE
                                                                                : SERD_NTRIPLES,
                                         (SerdStyle)(SERD_STYLE_ABBREVIATED | SERD_STYLE_RESOLVED | SERD_STYLE_CURIED),
                                         sordWorld().prefixes().c_obj(),
                                         &base_uri,
                                         Sord::string_sink, &result) ;
    serd_env_foreach(sordWorld().prefixes().c_obj(), (SerdPrefixSink)serd_writer_set_prefix, writer) ;
    for (auto const &prefix : prefixes) {
      serd_writer_set_prefix(writer, prefix.name().node()->to_serd_node(),
                                     prefix.uri().node()->to_serd_node()) ;
      }
    sord_write(m_model, writer, 0) ;
    serd_writer_free(writer) ;
    }
  else {                                   // Use Raptor
    raptor_uri *base_uri = NULL ;
    if (base_str) base_uri = raptor_new_uri(raptorWorld(), base_str) ;

    raptor_serializer *serialiser = raptor_new_serializer(raptorWorld(), get_raptor_format(format)) ;
//    if (serialiser == NULL) throw Exception("Unable to create graph serialiser") ;
    serd_env_foreach(sordWorld().prefixes().c_obj(), (SerdPrefixSink)set_raptor_prefix, serialiser) ;
    for (auto const &prefix : prefixes) {
      set_raptor_prefix((SerdWriter *)serialiser, prefix.name().node()->to_serd_node(),
                                                  prefix.uri().node()->to_serd_node()) ;
      }

    size_t length = 0 ;
    unsigned char *serialised ;
    raptor_serializer_start_to_string(serialiser, base_uri, (void **)(&serialised), &length) ;

    SordIter *statements = sord_begin(m_model) ;
    if (statements != NULL) {
      do {
        SordQuad statement ;
        sord_iter_get(statements, statement) ;
        raptor_statement *triple = raptor_new_statement(raptorWorld()) ;
        triple->subject = raptor_term_from_sord_node(statement[SORD_SUBJECT]) ;
        triple->predicate = raptor_term_from_sord_node(statement[SORD_PREDICATE]) ;
        triple->object = raptor_term_from_sord_node(statement[SORD_OBJECT]) ;
        raptor_serializer_serialize_statement(serialiser, triple) ;
        raptor_free_statement(triple) ;
        } while (!sord_iter_next(statements)) ;
      sord_iter_free(statements) ;
      }
    raptor_serializer_serialize_end(serialiser) ;
    result = std::string((char *)serialised, length) ;
    if (base_uri) raptor_free_uri(base_uri) ;
    raptor_free_serializer(serialiser) ;
    }
  return result ;
  }
