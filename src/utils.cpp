#include "utils.h"

#include "rdf.h"

#include <string>


rdf::URI utils::make_uri(const rdf::Node &uri)
/*==========================================*/
{
  return rdf::URI(uri.to_string()) ;
  }

rdf::URI utils::make_uri(const std::string &uri)
/*============================================*/
{
  return rdf::URI(uri) ;
  }


rdf::Node utils::datetime_to_isoformat(const Datetime & dt)
/*=======================================================*/
{
  return rdf::Node() ;
  }

utils::Datetime utils::isoformat_to_datetime(const rdf::Node & node)
/*================================================================*/
{
  return 0 ;
  }

