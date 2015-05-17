#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <typedobject/typedobject.h>

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

namespace example {

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

    RESTRICT_INTEGER(integer, 3)

    } ;

  } ;

#endif


