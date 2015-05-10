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

#ifndef TYPEDOBJECT_UTILS_H
#define TYPEDOBJECT_UTILS_H

#include "typedobject_export.h"
#include "rdf.h"
#include "rdfdefs.h"


#include <stdexcept>
#include <string>


namespace utils {

  class DatetimeImpl ;           // Declare forward
  class DurationImpl ;           // Declare forward


  class TimeException : public std::range_error
  /*-----------------------------------------*/
  {
   public:
    TimeException(const std::string &reason)
     : std::range_error(reason) { }
    } ;


  class TYPEDOBJECT_EXPORT Datetime
  /*------------------------------*/
  {
   public:
    Datetime() ;
    Datetime(const std::string &dt) ;
    Datetime(const rdf::Node &node) ;
    virtual ~Datetime() ;
    std::string to_string(void) const ;
    rdf::Literal to_literal(void) ;
    bool is_valid(void) const ;
   private:
    DatetimeImpl *m_datetime ;
    } ;


  class TYPEDOBJECT_EXPORT Duration
  /*-----------------------------*/
  {
   public:
    Duration() ;
    Duration(const std::string &dt, const bool strict=false) ;
    Duration(const rdf::Node &node) ;
    virtual ~Duration() ;
    std::string to_string(void) const ;
    rdf::Literal to_literal(void) ;
    bool is_valid(void) const ;
   private:
    DurationImpl *m_duration ;
    } ;

  } ;

#endif
