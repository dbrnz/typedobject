#ifndef _RDF_DEFS_H
#define _RDF_DEFS_H

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
  } ;

  class PRV {
   public:
    static const Namespace NS ;
    static const URI precededBy ;
    } ;

} ;

#endif
