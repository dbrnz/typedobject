#include "rdf/rdf.h"
#include "rdf/defs.h"

namespace bsml {

// bsml.h
  class BSML {
   public:
    static const rdf::Namespace NS ;
    static const rdf::URI Recording ;
    } ;

// bsml.cpp
  const rdf::Namespace BSML::NS("bsml", "http://www.biosignalml.org/ontologies/2011/04/biosignalml#") ;
  const rdf::URI BSML::Recording = BSML::NS.make_URI("Recording") ;

  } ;


// recording.h

#include "aobject.h"


namespace model {

  class Recording : public AObject::AObject
  /*-------------------------------------*/
  {
    A_OBJECT(bsml::BSML::Recording, rdf::RDFS::label)

    } ;


  } ;



// aoc_recording.cpp

namespace model {

  // We need to construct a list of metaclasses that reflect the class hierarchy order
  
//  const std::list<const rdf::URI> Recording::metaorder(void)
//  {
//    return { bsml::BSML::Recording } ;
//    }

  } ;
