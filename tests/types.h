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

#ifndef TEST_TYPES
#define TEST_TYPES

#include "typedobject/typedobject.h"

#include <string>

using namespace rdf ;

class TEST {
 public:
  static const Namespace NS ;
  static const URI string ;
  static const URI decimal ;
  static const URI integer ;
  static const URI object ;
  } ;

namespace test {

  class Object : public TypedObject::TypedObject
  /*------------------------------------------*/
  {
    TYPED_OBJECT(Object, OWL::Object)

    PROPERTY_STRING(string, TEST::string)
    PROPERTY_DECIMAL(decimal, TEST::decimal)
    PROPERTY_INTEGER(integer, TEST::integer)
    PROPERTY_DATETIME(starttime, DCT::created)
    PROPERTY_DURATION(duration, DCT::extent)
    PROPERTY_NODE(investigator, DCT::creator)

   private:
    std::string m_local ;
    int m_int ;
    INITIALISE(             \
      m_local = m_string ;  \
      m_int = m_integer ;   \
      )
    } ;


//  class RelativeTimeLine : public Object
//  /*----------------------------------*/
//  {
//    TYPED_OBJECT(RelativeTimeLine, TL::RelativeTimeLine)
//    } ;
//
// xx
//  class Signal : public Object
//  /*------------------------*/
//  {
//    TYPED_OBJECT(Signal, BSML::Signal)
//    PROPERTY_URI(recording, BSML::recording)
//    PROPERTY_URI(units, BSML::units)
//    PROPERTY_NODE(sensor, BSML::sensor)
//    PROPERTY_NODE(filter, BSML::preFilter)
//    PROPERTY_DOUBLE(rate, BSML::rate)
//    PROPERTY_DOUBLE(period, BSML::period)
//    PROPERTY_OBJECT(clock, BSML::clock, Clock)
//    PROPERTY_DOUBLE(minFrequency, BSML::minFrequency)
//    PROPERTY_DOUBLE(maxFrequency, BSML::maxFrequency)
//    PROPERTY_DOUBLE(minValue, BSML::minValue)
//    PROPERTY_DOUBLE(maxValue, BSML::maxValue)
//    PROPERTY_INTEGER(dataBits, BSML::dataBits)
//    PROPERTY_NODE(signaltype, BSML::signalType)
//    PROPERTY_DURATION(offset, BSML::offset)
//    PROPERTY_DURATION(duration, DCT::extent)
//    } ;
//
//
//  class Recording : public Object
//  /*---------------------------*/
//  {
//    TYPED_OBJECT(Recording, BSML::Recording)
//
//    PROPERTY_STRING(format, DCT::format)
//    PROPERTY_DATETIME(starttime, DCT::created)
//    PROPERTY_DURATION(duration, DCT::extent)
//    PROPERTY_NODE(investigator, DCT::creator)
//    
//    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
//
//    PROPERTY_NODE_SET(source, DCT::source)
//    PROPERTY_OBJECT_RSET(signals, BSML::recording, Signal)
//
//    } ;
//
//
//  class TemporalEntity : public Object
//  /*--------------------------------*/
//  {
//    TYPED_OBJECT(TemporalEntity, TIME::TemporalEntity)
//
//    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
//    PROPERTY_DURATION(start, NONE)
//    } ;
//
//  class Interval : public TemporalEntity
//  /*----------------------------------*/
//  {
//    TYPED_OBJECT(Interval, BSML::Interval)
//
//    ASSIGN_DURATION(start, TL::start)
//    } ;
//
//
//  class Instant : public TemporalEntity
//  /*---------------------------------*/
//  {
//    TYPED_OBJECT(Instant, BSML::Instant)
//
//    ASSIGN_DURATION(start, TL::at)
//    } ;


  } ;


#endif
