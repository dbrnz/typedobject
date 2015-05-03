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

#include <boost/date_time/posix_time/posix_time.hpp> // _types.hpp>

#include <stdexcept>
#include <string>


namespace utils {

  class TYPEDOBJECT_EXPORT TimeException : public std::range_error
  /*------------------------------------------------------------*/
  {
   public:
    TimeException(const std::string &reason)
     : std::range_error(reason) { }
    } ;

  class TYPEDOBJECT_EXPORT Datetime
  /*------------------------------*/
  {
   public:
    Datetime(): m_time(boost::date_time::not_a_date_time) { }
    Datetime(const std::string &dt) ;
    Datetime(const rdf::Node &node) ;
    std::string to_string(void) const ;
    rdf::Literal to_literal(void) ;
    bool is_valid(void) const ;
   private:
    boost::posix_time::ptime m_time ;
    } ;

  class TYPEDOBJECT_EXPORT Duration
  /*-----------------------------*/
  {
   public:
    Duration(): m_duration(boost::date_time::not_a_date_time) { }
    Duration(const std::string &dt, const bool strict=false) ;
    Duration(const rdf::Node &node) ;
    std::string to_string(void) const ;
    rdf::Literal to_literal(void) ;
    bool is_valid(void) const ;
   private:
    boost::posix_time::time_duration m_duration ;
    static const char *scan_decimal(const char *s, long *number, long *fraction, long *places,
                                    const char terminator) ;
    static const char *scan_integer(const char *s, long *result, const char terminator) ;
    } ;

  } ;

#endif
