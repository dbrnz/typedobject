#ifndef RDF_RDF_H
#define RDF_RDF_H

//**************************************************************************//

#include <sord/sordmm.hpp>

#include <string>
#include <list>
#include <cmath>

//**************************************************************************//

namespace rdf {

  typedef int64_t Integer ;
  /*---------------------*/

  typedef double Decimal ;
  /*--------------------*/

  typedef Sord::Node Node ;
  /*---------------------*/


  class URI : public Node
  /*-------------------*/
  {
   public:
    URI() ;
    URI(const std::string &uri) ;
    URI(const Node &uri) ;
    } ;


  class BNode : public Node
  /*---------------------*/
  {
   public:
    BNode() ;
    BNode(const std::string &s) ;
    } ;


  class Literal : public Node
  /*-----------------------*/
  {
   public:
    Literal() ;
    Literal(const std::string &s) ;
    Literal(const std::string &s, const rdf::URI &datatype) ;
    Literal(const std::string &s, const std::string &language) ;
    Literal(Decimal d, unsigned frac_digits) ;
    Literal(Integer i) ;

    class Constants {
     public:
      static const std::string EMPTY_STRING ;
      static const rdf::Decimal EMPTY_DECIMAL ;
      static const rdf::Integer EMPTY_INTEGER ;
      } ;
    inline static bool not_empty(const std::string &s) { return (s != Constants::EMPTY_STRING) ; }
    inline static bool not_empty(const Decimal d) { return std::isfinite(d) ; }
    inline static bool not_empty(const Integer i) { return (i != Constants::EMPTY_INTEGER) ; }

   private:
    static SordNode *sord_datatype_node(const std::string &s, const rdf::URI &datatype) ;
    static SordNode *sord_language_node(const std::string &s, const std::string &language) ;
    static SordNode *sord_decimal_node(Decimal d, unsigned frac_digits) ;
    static SordNode *sord_integer_node(Integer i) ;
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
    SordQuad m_quad ;
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

    const bool insert(const Statement &statement) const ;
    const bool insert(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const ;

    void addStatements(const StatementIter &statements) const ;

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
