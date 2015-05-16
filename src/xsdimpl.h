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

#ifndef TYPEDOBJECT_XSDIMPL_H
#define TYPEDOBJECT_XSDIMPL_H

#include <boost/date_time/posix_time/posix_time.hpp> // _types.hpp>

#include <string>


namespace xsd {

  class DatetimeImpl
  /*--------------*/
  {
   public:
    DatetimeImpl() ;
    DatetimeImpl(const std::string &dt) ;
    std::string to_string(void) const ;
    bool is_valid(void) const ;

   private:
    boost::posix_time::ptime m_ptime ;
    } ;


  class DurationImpl
  /*--------------*/
  {
   public:
    DurationImpl() ;
    DurationImpl(const std::string &dt, const bool strict=false) ;
    std::string to_string(void) const ;
    bool is_valid(void) const ;

   private:
    boost::posix_time::time_duration m_duration ;
    const char *scan_decimal(const char *s, long *number, long *fraction, long *places,
                             const char terminator) ;
    const char *scan_integer(const char *s, long *result, const char terminator) ;
    } ;

  } ;

#endif
