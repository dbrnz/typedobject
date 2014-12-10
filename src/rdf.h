#ifndef RDF_RDF_H
#define RDF_RDF_H

//**************************************************************************//

#include <sord/sordmm.hpp>

#include <string>
#include <list>


//**************************************************************************//

namespace rdf {


  typedef Sord::Node Node ;
  /*---------------------*/


  class URI : public Node
  /*-------------------*/
  {
   public:
    URI() ;
    URI(const std::string &uri) ;
    } ;


  class BNode : public Node
  /*---------------------*/
  {
   public:
    BNode(void) ;
    BNode(const std::string &s) ;
    } ;


  class Literal : public Node
  /*-----------------------*/
  {
   public:
    Literal(const std::string &s) ;
    Literal(const std::string &s, const rdf::URI &datatype) ;
    Literal(const std::string &s, const std::string &language) ;
    Literal(double d, unsigned frac_digits) ;
    Literal(int64_t i) ;

   private:
    static SordNode *sord_datatype_node(const std::string &s, const rdf::URI &datatype) ;
    static SordNode *sord_language_node(const std::string &s, const std::string &language) ;
    static SordNode *sord_decimal_node(double d, unsigned frac_digits) ;
    static SordNode *sord_integer_node(int64_t i) ;
    static SordNode *sord_node_from_serd_node(const SerdNode *node, const SerdNode *type, const SerdNode *lang) ;
    } ;


  typedef Sord::Iter StatementIter ;
  /*------------------------------*/


  class Namespace
  /*-----------*/
  {
   public:
    Namespace(const std::string &name, const std::string &uri) ;

    const URI make_URI(const std::string &suffix) const ;

    Literal name(void) const ;
    URI uri(void) const ;

   private:
    const Literal m_name ;
    const URI m_uri ;
    } ;


  class Statement
  /*-----------*/
  {
   public:
    Statement(const Node &s, const Node &p, const Node &o) ;

    friend class Graph ;

   private:
    SordQuad quad ;
    } ;


  class Graph
  /*-------*/
  {
   public:
    Graph() ;
    Graph(const std::string &uri) ;
    virtual ~Graph() ;

    enum class Format {
      UNKNOWN = 0,
      RDFXML,
      TURTLE,
      NTRIPLES
      } ;

    void parseResource(const std::string &resource,
                       const Format format=Format::RDFXML,
                       const std::string &base="") ;
    void parseString(const std::string &source,
                     const Format format=Format::RDFXML,
                     const std::string &base="") ;

    std::string serialise(const Format format=Format::RDFXML,
                          const std::string &base="",
                          const std::list<Namespace> &prefixes=std::list<Namespace>()) ;

    const URI &getUri(void) const ;

    const bool contains(const Statement &p_statement) const ;
    const bool contains(const Node &s, const Node &p, const Node &o) const ;

    StatementIter getStatements(const Statement &pattern) const ;
    StatementIter getStatements(const Node &s, const Node &p, const Node &o) const ;

   private:
    const URI m_uri ;
    SordModel *m_model ;
    } ;


  } ;

//**************************************************************************//

#endif
