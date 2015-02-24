#include "typedobject.h"


namespace TypedObject {


  TypedObject::TypedObject()
  /*----------------------*/
  : m_uri()
  {
    }

  TypedObject::TypedObject(const std::string &uri)
  /*--------------------------------------------*/
  : m_uri(uri)
  {
    }

  TypedObject::TypedObject(const std::string &uri, const rdf::Graph &graph)
  /*---------------------------------------------------------------------*/
  : TypedObject(uri)
  {
    this->add_metadata(graph) ;
    }

  TypedObject::~TypedObject()
  /*---------------*/
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


  TypedObject *TypedObject::create(const rdf::URI &T, const std::string &uri)
  /*-----------------------------------------------------------------------*/
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

  std::string TypedObject::to_string(void) const
  /*------------------------------------------*/
  {
    return type().to_string() + ": <" + m_uri.to_string() + ">" ;
    }

  bool TypedObject::add_metadata(const rdf::Graph &graph)
  /*---------------------------------------------------*/
  {
    if (m_uri.is_valid()) {
      if (graph.contains(m_uri, rdf::RDF::type, type())) {   // Needs to be sub-classes
        rdf::StatementIter statements = graph.getStatements(m_uri, rdf::Node(), rdf::Node()) ;
        if (!statements.end()) {
          do {
            assign_from_rdf(graph, statements.get_predicate(), statements.get_object(), false) ;
            } while (!statements.next()) ;
          }
        rdf::StatementIter rstatements = graph.getStatements(rdf::Node(), rdf::Node(), m_uri) ;
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


  } ;
