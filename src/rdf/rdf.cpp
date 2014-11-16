//**************************************************************************//

#include "rdf.h"

//**************************************************************************//

#include <sord/sordmm.hpp>

#include <string>

//**************************************************************************//

static Sord::World& getWorld()
/*--------------------------*/
{
  static Sord::World instance;
  return instance;
  }

//**************************************************************************//

rdf::Node::Node()
/*-------------*/
: Sord::Node()
{
  }

rdf::Node::Node(Type t, const std::string& s)
/*-----------------------------------------*/
: Sord::Node(getWorld(), t, s)
{
  }

rdf::Node::Node(SordNode* node, bool copy)
/*--------------------------------------*/
: Sord::Node(getWorld(), node, copy)
{
  }


SordNode *rdf::Node::sord_node_from_serd_node(
/*------------------------------------------*/
  const SerdNode* node, const SerdNode *type, const SerdNode *lang)
{
  return ::sord_node_from_serd_node(getWorld().c_obj(), getWorld().prefixes().c_obj(), node, type, lang) ;
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
: m_model(sord_new(getWorld().c_obj(), SORD_SPO | SORD_OPS, false)),
  m_uri(p_uri)
{
  }

rdf::Graph::~Graph()
/*----------------*/
{
  sord_free(m_model) ;
  }

void rdf::Graph::parseResource(
/*---------------------------*/
  const std::string &resource, const rdf::Format format, const std::string &base)
{
  uint8_t* path ;
  if (format != rdf::Format::TURTLE
   || (path = serd_file_uri_parse((const uint8_t*)resource.c_str(), NULL)) == NULL) {  // Use Raptor...

    }
  else {
    SerdReader* reader = sord_new_reader(m_model, getWorld().prefixes().c_obj(), SERD_TURTLE, NULL) ;
    serd_reader_read_file(reader, path) ;
    serd_reader_free(reader) ;
    free(path) ;
    }
  }

void rdf::Graph::parseString(
/*-------------------------*/
  const std::string &source, const rdf::Format format, const std::string &base)
{
  if (format == rdf::Format::TURTLE) {
    SerdReader* reader = sord_new_reader(m_model, getWorld().prefixes().c_obj(), SERD_TURTLE, NULL) ;
    serd_reader_read_string(reader, (const uint8_t*)source.c_str()) ;
    serd_reader_free(reader) ;
    }
  }


std::string rdf::Graph::serialise(
/*-------------------------------*/
  const rdf::Format &format, const std::string &base, const rdf::Prefixes &prefixes)
{
  std::string result = "" ;
  if (format == rdf::Format::TURTLE) {
    SerdURI base_uri = SERD_URI_NULL ;
    const uint8_t *base_str = (base != "") ? (const uint8_t*)base.c_str()
                            : (m_uri != URI::EMPTY) ? m_uri.to_u_string()
                            : NULL ;
    if (base_str) serd_uri_parse(base_str, &base_uri) ;

    SerdWriter* writer = serd_writer_new(SERD_TURTLE, SERD_STYLE_ABBREVIATED,
      getWorld().prefixes().c_obj(), &base_uri, Sord::string_sink, &result) ;
    serd_env_foreach(getWorld().prefixes().c_obj(), (SerdPrefixSink)serd_writer_set_prefix, writer) ;
    serd_env_foreach(prefixes.c_obj(), (SerdPrefixSink)serd_writer_set_prefix, writer) ;
    sord_write(m_model, writer, 0) ;
    serd_writer_free(writer) ;
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

  @classmethod
  def create_from_resource(cls, uri, format=Format.RDFXML, base=None):
  #-------------------------------------------------------------------
    """
    Create a new Graph from RDF statements in a resource.

    :param uri: The URI of RDF content to parse and add.
    :param format: The content's RDF format.
    :param base: An optional base URI of the content.
    :rtype: A :class:`Graph`
    """
    self = cls(uri)
    self.parse(source=uri, format=Format.name(format), publicID=base)
    return self

  @classmethod
  def create_from_string(cls, uri, string, format=Format.RDFXML):
  #--------------------------------------------------------------
    """
    Create a new Graph from RDF statements in a string.

    :param uri: The URI of the resulting graph. This is also
      use as the base URI when parsing.
    :param string: The RDF to parse and add.
    :type string: str
    :param format: The string's RDF format.
    :rtype: A :class:`Graph`
    """
    self = cls(uri)
    self.parse(data=string, format=Format.name(format), publicID=str(uri))
    return self

  def __str__(self):
  #-----------------
    return str(self.uri)

  def serialise(self, format=Format.RDFXML, base=None, prefixes={}):
  #-----------------------------------------------------------------
    '''
    Serialise the graph as a string of RDF statements.

    :param format: The RDF format to return.
    :param base: An optional base URI.
    :param prefixes: A dictionary of { 'prefix': 'namespace_uri' } abbreviations
      to use in the resulting serialisation.
    :type prefixes: dict
    :return: The graph serialised as a string.
    :rtype: str
    '''
    if base is None: base = self.uri
    for prefix, uri in prefixes.items():
      self.bind(prefix, uri)
    return self.serialize(format=Format.name(format)) ## BUG IN RDFLIB..., base=base)

#  def __iter__(self):
#  #------------------
#    return self.as_stream(self._default_graph).__iter__()

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
