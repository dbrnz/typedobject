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

#ifndef TYPEDOBJECT_XSD_H
#define TYPEDOBJECT_XSD_H

#include <typedobject/typedobject_export.h>

#include <stdexcept>
#include <string>


namespace rdf {
  class Node ;                   // Declare forward
  class Literal ;                // Declare forward
  } ;


namespace xsd {

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
    Datetime(const std::string & dt) ;
    Datetime(const rdf::Node & node) ;
    Datetime(const Datetime & other) ;
    Datetime(Datetime && other) ;
    virtual ~Datetime() ;
    Datetime & operator=(const Datetime & other) ;
    Datetime & operator=(Datetime && other) ;

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
    Duration(const std::string & dt, const bool strict=false) ;
    Duration(const rdf::Node & node) ;
    Duration(const Duration & other) ;
    Duration(Duration && other) ;
    virtual ~Duration() ;
    Duration & operator=(const Duration & other) ;
    Duration & operator=(Duration && other) ;

    std::string to_string(void) const ;
    rdf::Literal to_literal(void) ;
    bool is_valid(void) const ;
   private:
    DurationImpl *m_duration ;
    } ;

  } ;

#endif
