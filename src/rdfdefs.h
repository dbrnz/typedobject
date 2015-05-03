#ifndef TYPEDOBJECTS_RDFDEFS_H
#define TYPEDOBJECTS_RDFDEFS_H

#include "typedobject_export.h"
#include "rdf.h"


namespace rdf {

  class TYPEDOBJECT_EXPORT RDF {
   public:
    static const Namespace NS ;
    static const URI type ;
    } ;

  class TYPEDOBJECT_EXPORT RDFS {
   public:
    static const Namespace NS ;
    static const URI label ;
    static const URI comment ;
    } ;

  class TYPEDOBJECT_EXPORT XSD {
   public:
    static const Namespace NS ;
    static const URI dateTime ;
    static const URI dayTimeDuration ;
    } ;

  class TYPEDOBJECT_EXPORT OWL {
   public:
    static const Namespace NS ;
    static const URI Object ;
    } ;

  class TYPEDOBJECT_EXPORT DCT {
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

  class TYPEDOBJECT_EXPORT PRV {
   public:
    static const Namespace NS ;
    static const URI precededBy ;
    } ;

  class TYPEDOBJECT_EXPORT PROV {
   public:
    static const Namespace NS ;
    static const URI wasGeneratedBy ;
    } ;

  class TYPEDOBJECT_EXPORT TIME {
   public:
    static const Namespace NS ;
    static const URI TemporalEntity ;
    } ;

  class TYPEDOBJECT_EXPORT TL {
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
