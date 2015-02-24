#include "aobject.h"


namespace AObject {


  AObject::AObject()
  /*--------------*/
  : m_uri()
  {
    }

  AObject::AObject(const std::string &uri)
  /*------------------------------------*/
  : m_uri(uri)
  {
    }


  AObject::~AObject()
  /*---------------*/
  {
    }


  std::map<rdf::URI, AObjectFactory *> &AObject::m_factories(void)
  /*------------------------------------------------------------*/
  {
    static std::map<rdf::URI, AObjectFactory *> s_factories ;
    return s_factories ;
    }


  void AObject::register_type(const rdf::URI &T, AObjectFactory *factory)
  /*-------------------------------------------------------------------*/
  {
    AObject::m_factories()[T] = factory ;
    }


  AObject *AObject::create(const rdf::URI &T, const std::string &uri)
  /*---------------------------------------------------------------*/
  {
    return AObject::m_factories()[T]->create(uri) ;
    }


  bool AObject::operator==(const AObject &other) const
  /*------------------------------------------------*/
  {
    return m_uri.operator==(other.uri()) ;
    }

  bool AObject::operator<(const AObject &other) const
  /*-----------------------------------------------*/
  {
    return m_uri.operator<(other.uri()) ;
    }

  std::string AObject::to_string(void) const
  /*--------------------------------------*/
  {
    return type().to_string() + ": <" + m_uri.to_string() + ">" ;
    }

  bool AObject::add_metadata(const rdf::Graph &graph)
  /*-----------------------------------------------*/
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


  AObject *AObject::create_from_graph(const rdf::Graph &p_graph, const rdf::URI &p_uri)
  /*---------------------------------------------------------------------------------*/
  {

/*    get type from graph: (uri a ?type)

    look up registered types to get class creator

    creator->create(uri)

    (BSML::Recording, new RecordingFactory()) ;

*/



/*    AObject *self = new AObject(p_uri) ;   // But needs to be object with metaclass()
                                           // matching rdf:type
    self->addMetadata(p_graph) ;
    self->setGraph(p_graph.getUri()) ;
    return self ;
    */ return NULL ;
    }

  void AObject::set_graph(const rdf::URI &p_graph)
  /*--------------------------------------------*/
  {

    }


  } ;
