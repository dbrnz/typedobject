/*****************************************************************************
 *                                                                           *
 *  Typed RDF Objects for C++                                                *
 *                                                                           *
 *  Copyright (c) 2014-2015  David Brooks                                    *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 *****************************************************************************/

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
