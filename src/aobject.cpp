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


  void AObject::addMetadata(const rdf::Graph &graph)
  /*----------------------------------------------*/
  {
    if (m_uri.is_valid() && graph.contains(m_uri, rdf::RDF::type, metaclass())) {   // Needs to be sub-classes
                                                                 // ==> virtual ?? 
      rdf::StatementIter statements = graph.getStatements(m_uri, rdf::Node(), rdf::Node()) ;
      if (!statements.end()) {
        do {
          assign_from_rdf(graph, statements.get_predicate(), statements.get_object()) ;
          } while (!statements.next()) ;
        }
      }
    }

/*
class ClassFactory
{
  virtual AObject *create(void) = 0 ;
  } ;

class RecordingFactory : public ClassFactory
{
  Recording *create(void) { return new Recording }
  } ;

std::map<rdf::URI, ClassFactory *>

*/

  AObject *AObject::createFromGraph(const rdf::Graph &p_graph, const rdf::URI &p_uri)
  /*-------------------------------------------------------------------------------*/
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

  void AObject::setGraph(const rdf::URI &p_graph)
  /*-------------------------------------------*/
  {

    }


  } ;
