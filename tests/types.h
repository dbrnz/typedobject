#ifndef TEST_TYPES
#define TEST_TYPES

#include "typedobject.h"

#include <string>

using namespace rdf ;


namespace test {

  class Object : public TypedObject::TypedObject
  /*------------------------------------------*/
  {
    TYPED_OBJECT(Object, OWL::Object)

    PROPERTY_STRING(label, RDFS::label)
    PROPERTY_DATETIME(starttime, DCT::created)
    PROPERTY_DURATION(duration, DCT::extent)
    PROPERTY_NODE(investigator, DCT::creator)
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
