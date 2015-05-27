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

#include "types.h"

#include <iostream>

const Namespace TEST::NS("test", "http://example.org/properties#") ;
const URI TEST::string = TEST::NS.make_URI("string") ;
const URI TEST::decimal = TEST::NS.make_URI("decimal") ;
const URI TEST::integer = TEST::NS.make_URI("integer") ;
const URI TEST::object = TEST::NS.make_URI("object") ;


int main(void)
/*----------*/
{
  test::Object object(rdf::URI("http://example.org/object")) ;

  object.set_string("Some string") ;
  object.set_integer(3) ;

  object.set_investigator(rdf::URI("http://example.org/investigator")) ;

  //  PROPERTY_DATETIME(starttime, DCT::created)
  //  PROPERTY_DURATION(duration, DCT::extent)

  std::cout << object.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  }
