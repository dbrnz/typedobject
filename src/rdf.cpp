//**************************************************************************//

#include "rdf.h"

//**************************************************************************//

#include <sord/sordmm.hpp>
#include <raptor2.h>

//**************************************************************************//

#include <string>
#include <stdio.h>

//**************************************************************************//

static Sord::World& sordWorld()
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

rdf::Node::Node()
/*-------------*/
: Sord::Node()
{
  }

rdf::Node::Node(Type t, const std::string& s)
/*-----------------------------------------*/
: Sord::Node(sordWorld(), t, s)
{
  }

rdf::Node::Node(SordNode* node, bool copy)
/*--------------------------------------*/
: Sord::Node(sordWorld(), node, copy)
{
  }


SordNode *rdf::Node::sord_node_from_serd_node(
/*------------------------------------------*/
  const SerdNode* node, const SerdNode *type, const SerdNode *lang)
{
  return ::sord_node_from_serd_node(sordWorld().c_obj(), sordWorld().prefixes().c_obj(), node, type, lang) ;
  }


//**************************************************************************//

rdf::URI::URI()
/*-----------*/
: rdf::Node(rdf::Node::sord_node_from_serd_node(&SERD_NODE_NULL, NULL, NULL), true)
{
  }

rdf::URI::URI(const std::string& s)
/*-------------------------------*/
: rdf::Node(rdf::Node::URI, s)
{
  }

const rdf::URI rdf::URI::EMPTY ;
/*----------------------------*/


//**************************************************************************//

rdf::BNode::BNode(void)
/*-------------------*/
: rdf::Node(rdf::Node::BLANK, "")
{
  }

rdf::BNode::BNode(const std::string& s)
/*-----------------------------------*/
: rdf::Node(rdf::Node::BLANK, s)
{
  }

//**************************************************************************//

rdf::Literal::Literal(const std::string& s)
/*---------------------------------------*/
: rdf::Node(rdf::Node::LITERAL, s)
{
  }


rdf::Literal::Literal(const std::string& s, const rdf::URI &datatype)
/*-----------------------------------------------------------------*/
: rdf::Node(sord_datatype_node(s, datatype), false)
{
  }

rdf::Literal::Literal(const std::string& s, const std::string &language)
/*--------------------------------------------------------------------*/
: rdf::Node(sord_language_node(s, language), false)
{
  }

rdf::Literal::Literal(double d, unsigned frac_digits)
/*-------------------------------------------------*/
: rdf::Node(sord_decimal_node(d, frac_digits), false)
{
  }

rdf::Literal::Literal(int64_t i)
/*----------------------------*/
: rdf::Node(sord_integer_node(i), false)
{
  }


SordNode *rdf::Literal::sord_datatype_node(const std::string& s, const rdf::URI &datatype)
/*--------------------------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_from_string(SERD_LITERAL, (const uint8_t*)s.c_str()) ;
  const SerdNode *type = sord_node_to_serd_node(datatype.c_obj()) ;
  return rdf::Node::sord_node_from_serd_node(&val, type, NULL) ;
  }

SordNode *rdf::Literal::sord_language_node(const std::string& s, const std::string &language)
/*-----------------------------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_from_string(SERD_LITERAL, (const uint8_t*)s.c_str()) ;
  const SerdNode lang = serd_node_from_string(SERD_LITERAL, (const uint8_t*)language.c_str()) ;
  return rdf::Node::sord_node_from_serd_node(&val, NULL, &lang) ;
  }

SordNode *rdf::Literal::sord_decimal_node(double d, unsigned frac_digits)
/*---------------------------------------------------------------------*/
{
  const SerdNode val = serd_node_new_decimal(d, frac_digits) ;
  const SerdNode type = serd_node_from_string(SERD_URI, (const uint8_t*)SORD_NS_XSD "decimal") ;
  return rdf::Node::sord_node_from_serd_node(&val, &type, NULL) ;
  }

SordNode *rdf::Literal::sord_integer_node(int64_t i)
/*------------------------------------------------*/
{
  const SerdNode val = serd_node_new_integer(i) ;
  const SerdNode type = serd_node_from_string(SERD_URI, (const uint8_t*)SORD_NS_XSD "integer") ;
  return rdf::Node::sord_node_from_serd_node(&val, &type, NULL) ;
  }

//**************************************************************************//


rdf::Graph::Graph(const URI &p_uri)
/*-------------------------------*/
: m_model(sord_new(sordWorld().c_obj(), SORD_SPO | SORD_OPS, false)),
  m_uri(p_uri)
{
  }

rdf::Graph::~Graph()
/*----------------*/
{
  sord_free(m_model) ;
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
  SordQuad quad ;
  quad[SORD_SUBJECT] = sord_node_from_raptor_term(triple->subject) ;
  quad[SORD_PREDICATE] = sord_node_from_raptor_term(triple->predicate) ;
  quad[SORD_OBJECT] = sord_node_from_raptor_term(triple->object) ;
  quad[SORD_GRAPH] = NULL ;
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
  raptor_world_set_log_handler(raptorWorld(), NULL, process_raptor_message) ;
  raptor_parser *parser = raptor_new_parser(raptorWorld(), get_raptor_format(format)) ;
  raptor_parser_set_statement_handler(parser, model, process_raptor_statement) ;
  raptor_parser_set_namespace_handler(parser, model, process_raptor_namespace) ;
  if (base != "") *base_uri = raptor_new_uri(raptorWorld(), (uint8_t *)base.c_str()) ;
  return parser ;
  }


void rdf::Graph::parseResource(
/*---------------------------*/
  const std::string &resource, const rdf::Graph::Format format, const std::string &base)
{
  uint8_t* file_path = get_file_from_uri_string((const uint8_t*)resource.c_str()) ;

  if ((format != Format::TURTLE && format != Format::NTRIPLES) || file_path == NULL) {  // Use Raptor...
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
    SerdReader* reader = sord_new_reader(m_model, sordWorld().prefixes().c_obj(),
                                         (format == Format::TURTLE) ? SERD_TURTLE : SERD_NTRIPLES,
                                         NULL) ;
    serd_reader_read_file(reader, file_path) ;
    serd_reader_free(reader) ;
    }
  if (file_path != NULL) free(file_path) ;
  }

void rdf::Graph::parseString(
/*-------------------------*/
  const std::string &source, const rdf::Graph::Format format, const std::string &base)
{
  if (format == Format::TURTLE || format == Format::NTRIPLES) {  // Use sord
    SerdReader* reader = sord_new_reader(m_model, sordWorld().prefixes().c_obj(),
                                         (format == Format::TURTLE) ? SERD_TURTLE : SERD_NTRIPLES,
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

std::string rdf::Graph::serialise(
/*-------------------------------*/
  const rdf::Graph::Format format, const std::string &base, const rdf::Prefixes &prefixes)
{
  std::string result = "" ;
  const uint8_t *base_str = (base != "") ? (const uint8_t*)base.c_str()
                          : (m_uri != URI::EMPTY) ? m_uri.to_u_string()
                          : NULL ;
  if (format == Format::TURTLE || format == Format::NTRIPLES) {     // Use sord
    SerdURI base_uri = SERD_URI_NULL ;
    if (base_str) serd_uri_parse(base_str, &base_uri) ;

    SerdWriter* writer = serd_writer_new((format == Format::TURTLE) ? SERD_TURTLE : SERD_NTRIPLES,
                                         (SerdStyle)(SERD_STYLE_ABBREVIATED | SERD_STYLE_RESOLVED | SERD_STYLE_CURIED),
                                         sordWorld().prefixes().c_obj(),
                                         &base_uri,
                                         Sord::string_sink, &result) ;
    serd_env_foreach(sordWorld().prefixes().c_obj(), (SerdPrefixSink)serd_writer_set_prefix, writer) ;
    serd_env_foreach(prefixes.c_obj(), (SerdPrefixSink)serd_writer_set_prefix, writer) ;

    sord_write(m_model, writer, 0) ;
    
    serd_writer_free(writer) ;
    }
  else {                                   // Use Raptor
    raptor_uri *base_uri = NULL ;
    if (base_str) base_uri = raptor_new_uri(raptorWorld(), base_str) ;

    raptor_serializer *serialiser = raptor_new_serializer(raptorWorld(), get_raptor_format(format)) ;
//    if (serialiser == NULL) throw Exception("Unable to create graph serialiser") ;

    serd_env_foreach(sordWorld().prefixes().c_obj(), (SerdPrefixSink)set_raptor_prefix, serialiser) ;
    serd_env_foreach(prefixes.c_obj(), (SerdPrefixSink)set_raptor_prefix, serialiser) ;

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


const rdf::URI &rdf::Graph::getUri(void) const
/*------------------------------------------*/
{
  return m_uri ;
  }


/******************
class Graph(rdflib.graph.Graph):
#===============================

  def add_statements(self, statements):
  #------------------------------------
    '''
    Add statements to the graph.

    :param statements: A sequence of :class:`Statement`\s to add.
    :type statements: iterator
    '''
    for s in statements: self.add(s)

  def append_graph(self, graph):
  #-----------------------------
    '''
    Add statements from another graph to this graph.

    :param graph: A :class:`Graph` containing statements.
    '''
    if graph is not None:
      for s in graph: self.add(s)

  def contains(self, statement):
  #-----------------------------
    '''
    Test if a statement is in the graph.

    :param statement: The statement to check. Some or all of the `subject`,
      `predicate` or `object` attributes can be `None`, meaning they match any value.
    :type statement: :class:`Statement`
    :return: True if the graph contains `statement`.
    :rtype: bool
    '''
    return statement in self

  def has_resource(self, uri, rtype):
  #----------------------------------
    '''
    Does the graph contain a resource of the given type?
    '''
    return self.contains(Statement(uri, RDF.type, rtype))

  def get_statements(self, statement):
  #-----------------------------------
    '''
    Get all matching statements in the graph.

    :param statement: The statement to find. Some or all of the `subject`,
      `predicate` or `object` attributes can be `None`, meaning they match any value.
    :type statement: :class:`Statement`
    :return: A sequence of :class:`Statement`\s.
    '''
    for s in self.triples(statement): yield Statement(*s)

  def get_object(self, s, p):
  #--------------------------
    '''
    Get the object of a (`subject`, `predicate`) pair. One or both of `subject`
    and `predicate` may be `None`, meaning they match any value.

    :param s: The `subject` of the statement.
    :param p: The `predicate` of the statement.
    :return: The `object` node if the statement (`subject`, `predicate`, `object`) is
      in the graph, otherwise `None`.
    :rtype: :class:`Node`
    '''
    return tuple(self.get_objects(s, p))[0]

  def get_literal(self, s, p):
  #----------------------------
    '''
    Get the object of a (`subject`, `predicate`) pair as a string if it is a
    :class:`Literal`. One or both of `subject` and `predicate` may be `None`,
    meaning they match any value.

    :param s: The `subject` of the statement to lookup.
    :param p: The `predicate` of the statement to lookup.
    :return: The `object` as a string if the statement (`subject`, `predicate`, `object`)
      is in the graph and `object` is a :class:`Literal`; as a :class:`Node` if it
      exists and is not a Literal; otherwise `None`.
    '''
    l = self.get_object(s, p)
    return l.as_string() if l else None

  def get_subjects(self, p, o):
  #----------------------------
    '''
    Get a sequence of subjects matching the pair (`predicate`, `object`). One or
    both of `predicate` and `object` may be `None`, meaning they match any value.

    :param p: The `predicate` of the statement to lookup.
    :param o: The `object` of the statement to lookup.
    :return: An iterator yielding a sequence of `subject` nodes with
      (`subject`, `predicate`, `object`) statements in the graph.
    :rtype: iterator
    '''
    if p and not isinstance(p, rdflib.term.URIRef): p = Resource(p)
    return self.subjects(predicate=p, object=o)

  def get_objects(self, s, p):
  #---------------------------
    '''
    Get a sequence of objects matching the pair (`subject`, `predicate`). One or
    both of `subject` and `predicate` may be `None`, meaning they match any value.

    :param s: The `subject` of the statement to lookup.
    :param p: The `predicate` of the statement to lookup.
    :return: An iterator yielding a sequence of `object` nodes with
      (`subject`, `predicate`, `object`) statements in the graph.
    :rtype: iterator
    '''
    if s and not isinstance(s, rdflib.term.URIRef): s = Resource(s)
    if p and not isinstance(p, rdflib.term.URIRef): p = Resource(p)
    return self.objects(subject=s, predicate=p)

  def get_literals(self, s, p):
  #------------------------------
    '''
    Get a sequence of objects matching the pair (`subject`, `predicate`), with the
    `object` as a string if it is a :class:`Literal`. One or both of `subject`
    and `predicate` may be `None`, meaning they match any value.

    :param s: The `subject` of the statement to lookup.
    :param p: The `predicate` of the statement to lookup.
    :return: An iterator yielding a sequence of strings and non-Literal :class:`Node`\s
       for `object`\s with (`subject`, `predicate`, `object`) statements in the graph.
    :rtype: iterator
    '''
    for v in self.get_objects(s, p): yield v.as_string()

  def set_subject_property(self, s, p, o):
  #---------------------------------------
    """
    Append the statement (s, p, o) to the graph after first removing
    all statements with subject `s` and predicate `p`.
    """
    self.remove(Statement(s, p, None))
    self.add(Statement(s, p, o))

  def query(self, sparql):
  #-----------------------
    '''
    Perform a SPARQL query against RDF statements in the graph.

    :param sparql: The SPARQL query.
    :type: str
    :return: An iterator of the results from the query.
    :rtype: :class:`QueryResults`
    '''
    try:
      return super(Graph, self).query(sparql)
    except Exception as msg:
      logging.error('Graph query: %s', msg)
    return [ ]

******************/
