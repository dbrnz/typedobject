#include "rdf/rdf.h"

#include <iostream>


int main(void)
/*----------*/
{
  rdf::URI g_uri("http://www.bsml.org/graph") ;
  rdf::Graph g(g_uri) ;

  std::string turtle = R"(
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix map:  <http://www.biosignalml.org/ontologies/2011/02/mapping#> .
@prefix :     <http://www.biosignalml.org/mappings/> .

:label
  a map:Mapping ;
  rdfs:label "label" ;
  map:property rdfs:label
  .

:comment
  a map:Mapping ;
  rdfs:comment "comment" ;
  map:property rdfs:comment
  .

:description
  a map:Mapping ;
  rdfs:label "description" ;
  map:property dcterms:description
  .

  )" ;

  g.parseString(turtle, rdf::Format::TURTLE) ;

  std::cout << g.serialise(rdf::Format::TURTLE) << std::endl ;

  }
