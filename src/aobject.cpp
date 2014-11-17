#include "aobject.h"
#include "rdf/rdf.h"


namespace AObject {

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
//    if (!m_metaclass.empty()
//      && p_graph.contains(m_uri, rdf::RDF::type, m_metaclass)) {

  //    for stmt in p_graph.getStatements(m_uri, None, None) {

  /*

          for metaclass in [getattr(cls, 'metaclass', None)
                              for cls in self.__class__.__mro__ if cls != object]:
            s, attr, v, fn = self.rdfmap.metadata(metaclass, stmt)
            #logging.debug("%s: %s = '%s'", self.uri, attr, v)  ###
            if attr is not None:
              self._assign(attr, v, fn)
              break
  */
  //    }
//      }
    }


  AObject *AObject::createFromGraph(const rdf::Graph &p_graph, const rdf::URI &p_uri)
  /*-------------------------------------------------------------------------------*/
  {
    AObject *self = new AObject(p_uri) ;

    self->addMetadata(p_graph) ;
    self->setGraph(p_graph.getUri()) ;
    return self;
    }

  void AObject::setGraph(const rdf::URI &p_graph)
  /*-------------------------------------------*/
  {

    }


  } ;
