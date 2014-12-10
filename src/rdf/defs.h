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

  class DCT {
   public:
    static const Namespace NS ;
    static const URI description ;
    static const URI creator ;
    static const URI created ;
    static const URI format ;
    static const URI source ;
    static const URI subject ;
  } ;

  class PRV {
   public:
    static const Namespace NS ;
    static const URI precededBy ;
    } ;

} ;

#endif
