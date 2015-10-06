#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <typedobject/typedobject.h>

#include <string>


namespace TEST {
  NAMESPACE("test", "http://www.example.org/test#")
  TERM(SubObject)

  TERM(string)
  TERM(decimal)
  TERM(integer)
  TERM(subobject)
  TERM(reference)
  } ;

namespace BSML {
  NAMESPACE("bsml", "http://www.biosignalml.org/ontologies/2011/04/biosignalml#")
  TERM(Interval)
  TERM(Instant)
  } ;


using namespace rdf ;


namespace example {


  class SubObject : public TypedObject::TypedObject
  /*---------------------------------------------*/
  {
    TYPED_OBJECT(SubObject, TEST::SubObject)
    } ;

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

    PROPERTY_OBJECT(subobject, TEST::subobject, SubObject)
    PROPERTY_REFERENCE(reference, TEST::reference, SubObject)

    PREFIXES(TEST::NS, DCT::NS)

/**
#define PROPERTY_URI(NAME, P)            _PROPERTY(NAME, P, rdf::URI)
#define PROPERTY_OBJECT(NAME, P, T)      _PROPERTY_OBJ(NAME, P, T)

// What is the difference between URI, NODE, and OBJECT properties??
// Could URI and NODE be combined? And renamed to RESOURCE??

#define PROPERTY_STRING_SET(NAME, P)     _PROPERTY_SET(NAME, P, std::string)
#define PROPERTY_NODE_SET(NAME, P)       _PROPERTY_SET(NAME, P, rdf::Node)
#define PROPERTY_URI_SET(NAME, P)        _PROPERTY_SET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_SET(NAME, P, T)  _PROPERTY_OBJ_SET(NAME, P, T, OBJ)

#define PROPERTY_URI_RSET(NAME, P)       _PROPERTY_RSET(NAME, P, rdf::URI)
#define PROPERTY_OBJECT_RSET(NAME, P, T) _PROPERTY_OBJ_RSET(NAME, P, T, OBJ)

#define ASSIGN_DATETIME(NAME, P)         _ASSIGN(NAME, P, utils::Datetime)
#define ASSIGN_DURATION(NAME, P)         _ASSIGN(NAME, P, utils::Duration)

#define RESTRICTION(NAME, VALUE)         _RESTRICTION(NAME, VALUE)
**/

    } ;

  class RestrictedObject : public Object
  /*----------------------------------*/
  {
    TYPED_OBJECT(RestrictedObject, OWL::Object)

    RESTRICT_INTEGER(integer, 3)   // This now applies to all OWL::Object classes
    // since this is last class seen so RDF owl:object ==> RestrictedObject
    } ;


  class RelativeTimeLine : public Object
  /*----------------------------------*/
  {
    TYPED_OBJECT(RelativeTimeLine, TL::RelativeTimeLine)
    } ;


  class TemporalEntity : public Object
  /*--------------------------------*/
  {
    TYPED_OBJECT(TemporalEntity, TIME::TemporalEntity)
    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
    PROPERTY_DURATION(start, NONE)
    PROPERTY_DURATION(duration, NONE)
    } ;


  class Interval : public TemporalEntity
  /*----------------------------------*/
  {
    TYPED_OBJECT(Interval, BSML::Interval)
    ASSIGN_DURATION(start, TL::start)
    ASSIGN_DURATION(duration, TL::duration)
    } ;

  class Instant : public TemporalEntity
  /*---------------------------------*/
  {
    TYPED_OBJECT(Instant, BSML::Instant)
    ASSIGN_DURATION(start, TL::at)
    } ;


  } ;

#endif


