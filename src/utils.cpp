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

#include "typedobject/utils.h"

#include <boost/date_time/posix_time/posix_time.hpp> // _types.hpp>

#include <math.h>
#include <sstream>

namespace utils {

  class DatetimeImpl
  /*--------------*/
  {
   public:

    DatetimeImpl()
    : m_ptime(boost::date_time::not_a_date_time)
    {
      }

    DatetimeImpl(const std::string &dt)
    : m_ptime(boost::posix_time::time_from_string(dt))
    {
      }

    std::string to_string(void) const
    {
      return boost::posix_time::to_iso_extended_string(m_ptime) ;
      }

    bool is_valid(void) const
    {
      return !m_ptime.is_special() ;
      }

   private:
    boost::posix_time::ptime m_ptime ;
    } ;


  class DurationImpl
  /*--------------*/
  {
   public:

    DurationImpl()
    : m_duration(boost::date_time::not_a_date_time)
    {
      }

    DurationImpl(const std::string &dt, const bool strict=false)
    {
      const char *s = dt.c_str() ;
      bool negative = (*s == '-') ;
      if (negative) ++s ;

      long days = 0 ;
      long hours = 0 ;
      long minutes = 0 ;
      long seconds = 0 ;  long fraction = 0 ;  long decimals = 0 ;
      boost::posix_time::time_duration td ;

      if (strict || *s == 'P') {  // XSD duration format
        if (*s != 'P') throw utils::TimeException("Invalid XSD duration: '" + dt + "'") ;
        ++s ;
        bool scanned = false ;
        if (*s == 'T') ++s ;
        else {
          s = scan_integer(s, &days, 'D') ;
          if (*s == 'T') ++s ;
          else if (*s == '\0') scanned = true ;
          else throw utils::TimeException("Invalid XSD duration: '" + dt + "'") ;
          }
        if (!scanned) {
          s = this->scan_integer(s, &hours, 'H') ;
          s = this->scan_integer(s, &minutes, 'M') ;
          s = this->scan_decimal(s, &seconds, &fraction, &decimals, 'S') ;
          }
        }
      else {                      // Assume seconds
        s = this->scan_decimal(s, &seconds, &fraction, &decimals, '\0') ;
        }
      if (*s != '\0') throw utils::TimeException("Invalid duration: '" + dt + "'") ;
      td = boost::posix_time::time_duration(24*days + hours, minutes, seconds,
                                            fraction*boost::posix_time::time_duration::ticks_per_second()
                                            / pow(10, decimals)) ;
      m_duration = negative ? td.invert_sign() : td ;
      }

    std::string to_string(void) const
    {
      std::stringstream ss("") ;
      if (!(m_duration.is_special() && m_duration.is_not_a_date_time())) {
        boost::posix_time::time_duration dt ;
        if (m_duration.is_negative()) {
          ss << "-" ;
          dt = m_duration.invert_sign() ;
          }
        else dt = m_duration ;
        ss << "P" ;
        long hours = dt.hours() ;
        long mins = dt.minutes() ;
        long secs = dt.seconds() ;
        long frac = dt.fractional_seconds() ;
        if (hours || mins || secs || frac) {
          if (secs >= 60) {
            mins += secs/60 ;
            secs %= 60 ;
            }
          if (mins >= 60) {
            hours += mins/60 ;
            mins %= 60 ;
            }
          if (hours >= 24) {
            ss << hours/24 << "D" ;
            hours %= 24 ;
            }
          if (hours || mins || secs || frac) ss << "T" ;
          if (hours) ss << hours << "H" ;
          if (mins) ss << mins << "M" ;
          if (secs || frac) {
            ss << secs ;
            if (frac) {
              while ((frac % 10) == 0) frac /= 10 ;
              ss << "." ;
              ss << frac ;
              }
            ss << "S" ;
            }
          }
        else ss << "T0S" ;
        }
      return ss.str() ;
      }

    bool is_valid(void) const
    {
      return !m_duration.is_special() ;
      }

   private:
    boost::posix_time::time_duration m_duration ;

    const char *scan_decimal(const char *s, long *number, long *fraction, long *places,
                             const char terminator)
    {
      *number = 0 ;
      *fraction = 0 ;
      *places = 0 ;
      if (isdigit(*s)) {
        char *end ;
        long i = strtol(s, &end, 10) ;
        if (*end == terminator) {
          *number = i ;
          return terminator ? (end + 1) : end ;
          }
        else if (*end == '.') {
          char *d = end + 1 ;
          if (isdigit(*d)) {
            long f = strtol(d, &end, 10) ;
            if (*end == terminator) {
              *number = i ;
              *fraction = f ;
              *places = (end - d) ;
              return terminator ? (end + 1) : end ;
              }
            }
          }
        }
      return s ;
      }

    const char *scan_integer(const char *s, long *result, const char terminator)
    {
      if (isdigit(*s)) {
        char *end ;
        long i = strtol(s, &end, 10) ;
        if (*end == terminator) {
          *result = i ;
          return end + 1 ;
          }
        }
      *result = 0 ;
      return s ;
      }

    } ;

  } ;




utils::Datetime::Datetime()
/*-----------------------*/
: m_datetime(nullptr)
{
  }

utils::Datetime::Datetime(const std::string & dt)
/*---------------------------------------------*/
: m_datetime(new DatetimeImpl(dt))
{
  }

utils::Datetime::Datetime(const rdf::Node & node)
/*---------------------------------------------*/
{
  if (!node.is_literal_type(rdf::XSD::dateTime.to_string().c_str()))
    throw utils::TimeException("Not a XSD dateTime: '" + node.to_string() + "'") ;
  m_datetime = new DatetimeImpl(node.to_string()) ;
  }

utils::Datetime::Datetime(const utils::Datetime & other)
/*----------------------------------------------------*/
: m_datetime((other.m_datetime != nullptr) ? new DatetimeImpl(*other.m_datetime) : nullptr)
{
  }

utils::Datetime::Datetime(utils::Datetime && other)
/*-----------------------------------------------*/
: m_datetime(other.m_datetime)
{
  other.m_datetime = nullptr;
  }

utils::Datetime::~Datetime()
/*------------------------*/
{
  if (m_datetime != nullptr) delete m_datetime ;
  }

utils::Datetime & utils::Datetime::operator=(const utils::Datetime & other)
/*-----------------------------------------------------------------------*/
{
  utils::Datetime datetime(other) ;
  *this = std::move(datetime) ;
  return *this ;
  }

utils::Datetime & utils::Datetime::operator=(utils::Datetime && other)
/*------------------------------------------------------------------*/
{
  std::swap(m_datetime, other.m_datetime) ;
  return *this ;
  }

std::string utils::Datetime::to_string(void) const
/*----------------------------------------------*/
{
  return (m_datetime == nullptr) ? "" : m_datetime->to_string() ;
  }

rdf::Literal utils::Datetime::to_literal(void)
/*------------------------------------------*/
{
  return rdf::Literal(this->to_string(), rdf::XSD::dateTime) ;
  }

bool utils::Datetime::is_valid(void) const
/*--------------------------------------*/
{
  return (m_datetime != nullptr) && m_datetime->is_valid() ;
  }




utils::Duration::Duration()
/*-----------------------*/
: m_duration(nullptr)
{
  }

utils::Duration::Duration(const std::string & dt, const bool strict)
/*----------------------------------------------------------------*/
: m_duration(new DurationImpl(dt, strict))
{
  }

utils::Duration::Duration(const rdf::Node & node)
/*---------------------------------------------*/
: utils::Duration(node.to_string(),
                  node.is_literal_type(rdf::XSD::dayTimeDuration.to_string().c_str()))

{
  }

utils::Duration::Duration(const utils::Duration & other)
/*----------------------------------------------------*/
: m_duration((other.m_duration != nullptr) ? new DurationImpl(*other.m_duration) : nullptr)
{
  }

utils::Duration::Duration(utils::Duration && other)
/*-----------------------------------------------*/
: m_duration(other.m_duration)
{
  other.m_duration = nullptr;
  }

utils::Duration::~Duration()
/*------------------------*/
{
  if (m_duration != nullptr) delete m_duration ;
  }

utils::Duration & utils::Duration::operator=(const utils::Duration & other)
/*-----------------------------------------------------------------------*/
{
  utils::Duration duration(other) ;
  *this = std::move(duration) ;
  return *this ;
  }

utils::Duration & utils::Duration::operator=(utils::Duration && other)
/*------------------------------------------------------------------*/
{
  std::swap(m_duration, other.m_duration) ;
  return *this ;
  }

std::string utils::Duration::to_string(void) const
/*----------------------------------------------*/
{
  return (m_duration == nullptr) ? "" : m_duration->to_string() ;
  }

rdf::Literal utils::Duration::to_literal(void)
/*------------------------------------------*/
{
  return rdf::Literal(this->to_string(), rdf::XSD::dayTimeDuration) ;
  }

bool utils::Duration::is_valid(void) const
/*--------------------------------------*/
{
  return (m_duration != nullptr) && m_duration->is_valid() ;
  }

