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

#include "typedobject/xsd.h"
#include "typedobject/rdf.h"
#include "typedobject/rdfdefs.h"
#include "xsdimpl.h"

#include <boost/date_time/posix_time/posix_time.hpp> // _types.hpp>


xsd::Datetime::Datetime()
/*-----------------------*/
: m_datetime(nullptr)
{
  }

xsd::Datetime::Datetime(const std::string & dt)
/*---------------------------------------------*/
: m_datetime(new DatetimeImpl(dt))
{
  }

xsd::Datetime::Datetime(const rdf::Node & node)
/*---------------------------------------------*/
{
  if (!node.is_literal_type(rdf::XSD::dateTime.to_string().c_str()))
    throw xsd::TimeException("Not a XSD dateTime: '" + node.to_string() + "'") ;
  m_datetime = new DatetimeImpl(node.to_string()) ;
  }

xsd::Datetime::Datetime(const xsd::Datetime & other)
/*----------------------------------------------------*/
: m_datetime((other.m_datetime != nullptr) ? new DatetimeImpl(*other.m_datetime) : nullptr)
{
  }

xsd::Datetime::Datetime(xsd::Datetime && other)
/*-----------------------------------------------*/
: m_datetime(other.m_datetime)
{
  other.m_datetime = nullptr;
  }

xsd::Datetime::~Datetime()
/*------------------------*/
{
  if (m_datetime != nullptr) delete m_datetime ;
  }

xsd::Datetime & xsd::Datetime::operator=(const xsd::Datetime & other)
/*-----------------------------------------------------------------------*/
{
  xsd::Datetime datetime(other) ;
  *this = std::move(datetime) ;
  return *this ;
  }

xsd::Datetime & xsd::Datetime::operator=(xsd::Datetime && other)
/*------------------------------------------------------------------*/
{
  std::swap(m_datetime, other.m_datetime) ;
  return *this ;
  }

std::ostream & xsd::operator<<(std::ostream & os, const xsd::Datetime & dt)
/*-----------------------------------------------------------------------*/
{
  os << dt.to_string() ;
  return os ;
  }

std::string xsd::Datetime::to_string(void) const
/*----------------------------------------------*/
{
  return (m_datetime == nullptr) ? "" : m_datetime->to_string() ;
  }

rdf::Literal xsd::Datetime::to_literal(void)
/*------------------------------------------*/
{
  return rdf::Literal(this->to_string(), rdf::XSD::dateTime) ;
  }

bool xsd::Datetime::is_valid(void) const
/*--------------------------------------*/
{
  return (m_datetime != nullptr) && m_datetime->is_valid() ;
  }




xsd::Duration::Duration()
/*-----------------------*/
: m_duration(nullptr)
{
  }

xsd::Duration::Duration(const std::string & dt, const bool strict)
/*----------------------------------------------------------------*/
: m_duration(new DurationImpl(dt, strict))
{
  }

xsd::Duration::Duration(const rdf::Node & node)
/*---------------------------------------------*/
: xsd::Duration(node.to_string(),
                  node.is_literal_type(rdf::XSD::dayTimeDuration.to_string().c_str()))

{
  }

xsd::Duration::Duration(const xsd::Duration & other)
/*----------------------------------------------------*/
: m_duration((other.m_duration != nullptr) ? new DurationImpl(*other.m_duration) : nullptr)
{
  }

xsd::Duration::Duration(xsd::Duration && other)
/*-----------------------------------------------*/
: m_duration(other.m_duration)
{
  other.m_duration = nullptr;
  }

xsd::Duration::~Duration()
/*------------------------*/
{
  if (m_duration != nullptr) delete m_duration ;
  }

xsd::Duration & xsd::Duration::operator=(const xsd::Duration & other)
/*-----------------------------------------------------------------------*/
{
  xsd::Duration duration(other) ;
  *this = std::move(duration) ;
  return *this ;
  }

xsd::Duration & xsd::Duration::operator=(xsd::Duration && other)
/*------------------------------------------------------------------*/
{
  std::swap(m_duration, other.m_duration) ;
  return *this ;
  }

std::ostream & xsd::operator<<(std::ostream & os, const xsd::Duration & d)
/*----------------------------------------------------------------------*/
{
  os << d.to_string() ;
  return os ;
  }

std::string xsd::Duration::to_string(void) const
/*----------------------------------------------*/
{
  return (m_duration == nullptr) ? "" : m_duration->to_string() ;
  }

rdf::Literal xsd::Duration::to_literal(void)
/*------------------------------------------*/
{
  return rdf::Literal(this->to_string(), rdf::XSD::dayTimeDuration) ;
  }

bool xsd::Duration::is_valid(void) const
/*--------------------------------------*/
{
  return (m_duration != nullptr) && m_duration->is_valid() ;
  }
