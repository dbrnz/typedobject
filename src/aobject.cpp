#include "aobject.h"

#include "rdf/rdf.h"
#include "rdf/defs.h"


namespace AObject {

// Generated...

  const rdf::URI &AObject::metaclass(void) const
  /*------------------------------------------*/
  {
    static rdf::URI m_metaclass ;
    return m_metaclass ;
    }

  void AObject::assign_from_rdf(const rdf::Node &property, const rdf::Node &value)
  /*----------------------------------------------------------------------------*/
  {
    //SUPER::from_rdf(property, value) ;
    if      (property == rdf::RDFS::label) label = value.to_string() ;
    else if (property == rdf::RDFS::comment) comment = value.to_string() ;
    else if (property == rdf::DCT::description) description = value.to_string() ;
    else if (property == rdf::PRV::precededBy) precededBy = value ;
    else if (property == rdf::DCT::creator) creator = value ;
    else if (property == rdf::DCT::created) creator = utils::isoformat_to_datatime(value.to_string()) ;
    }

//    virtual RDF::Node to_rdf(const rdf::Graph &graph)
//    /*----------------------------------------------*/
//    {
//      //SUPER::from_rdf(property, value) ;
//      if      (property == RDFS::label) label = value.to_string() ;
//      else if (property == DCT::creator) creator = value.to_uri() ;
//      }


// Hard-coded...

  AObject::AObject()
  /*--------------*/
  : m_uri()
  {
    }

  AObject::AObject(const rdf::URI &p_uri)
  /*-----------------------------------*/
  : m_uri(p_uri)
  {
    }

  AObject::~AObject()
  /*---------------*/
  {
    }


  void AObject::addMetadata(const rdf::Graph &p_graph)
  /*-------------------------------------------------*/
  {
    if (p_graph.contains(m_uri, rdf::RDF::type, metaclass())) {   // Needs to be sub-classes
                                                                 // ==> virtual ?? 
      rdf::StatementIterator statements = p_graph.getStatements(m_uri, rdf::URI(), rdf::Node()) ;
      while (!statements.end()) {
        assign_from_rdf(statements.get_predicate(), statements.get_object()) ;
        ++statements ;
        }
      }
/***
      for stmt in p_graph.getStatements(m_uri, None, None) {
// For all rdfmaps going forwards... (that aren't empty URIs)
          for metaclass in [getattr(cls, 'metaclass', None)
                              for cls in self.__class__.__mro__ if cls != object]:

            s, attr, v, fn = self.rdfmap.metadata(metaclass, stmt)
            #logging.debug("%s: %s = '%s'", self.uri, attr, v)  ###
            if attr is not None:
              self._assign(attr, v, fn)
              break
      }
**/
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
