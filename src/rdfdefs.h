#ifndef RDF_DEFS_H
#define RDF_DEFS_H

#include "rdf.h"


namespace rdf {

  class RDF {
   public:
    static const Namespace NS ;
    static const URI type ;
    } ;

  class RDFS {
   public:
    static const Namespace NS ;
    static const URI label ;
    static const URI comment ;
    } ;

  class XSD {
   public:
    static const Namespace NS ;
    static const URI dateTime ;
    static const URI dayTimeDuration ;
    } ;

  class OWL {
   public:
    static const Namespace NS ;
    static const URI Object ;
    } ;

  class DCT {
   public:
    static const Namespace NS ;
    static const URI description ;
    static const URI creator ;
    static const URI created ;
    static const URI format ;
    static const URI source ;
    static const URI subject ;
    static const URI extent ;
  } ;

  class PRV {
   public:
    static const Namespace NS ;
    static const URI precededBy ;
    } ;

  class PROV {
   public:
    static const Namespace NS ;
    static const URI wasGeneratedBy ;
    } ;

  class TIME {
   public:
    static const Namespace NS ;
    static const URI TemporalEntity ;
    } ;

  class TL {
   public:
    static const Namespace NS ;
    static const URI RelativeTimeLine ;
    static const URI timeline ;
    static const URI at ;
    static const URI start ;
    static const URI duration ;
    } ;

} ;

#endif
