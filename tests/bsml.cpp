#include "model.h"

#include <iostream>
#include <string>


int main(void)
/*----------*/
{

  std::string turtle = R"(
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix dct: <http://purl.org/dc/terms/> .
@prefix ns3: <http://purl.org/NET/c4dm/timeline.owl#> .
@prefix bsml: <http://www.biosignalml.org/ontologies/2011/04/biosignalml#> .
@prefix ns5: <http://www.biosignalml.org/ontologies/examples/physiobank#> .

<http://demo.biosignalml.org/physiobank/mitdb/102>
	ns3:timeline <http://demo.biosignalml.org/physiobank/mitdb/102/timeline> ;
	dct:extent "1806.6"^^<http://www.w3.org/2001/XMLSchema#dayTimeDuration> ;
	dct:format "application/x-bsml+hdf5" ;
	dct:source <http://physionet.org/physiobank/database/mitdb/102.hea> ;
	dct:subject <http://demo.biosignalml.org/physiobank/mitdb/102_patient> ;
	bsml:dataset <file:///recordings/physiobank/mitdb/102.h5> ;
	ns5:database <http://physionet.org/physiobank/database/mitdb> ;
	rdfs:label "MITDB 102..." ;
	a bsml:Recording .

<http://demo.biosignalml.org/physiobank/mitdb/102/annotation1>
	dct:subject <http://demo.biosignalml.org/physiobank/mitdb/102> ;
	a bsml:Annotation .

<http://demo.biosignalml.org/physiobank/mitdb/102/signal/ECG1>
	bsml:dataBits 11 ;
	bsml:rate 360 ;
	bsml:recording <http://demo.biosignalml.org/physiobank/mitdb/102> ;
	bsml:signalType bsml:ECG ;
	bsml:units <http://www.sbpax.org/uome/list.owl#Millivolt> ;
	a bsml:Signal ;
	rdfs:label "V5" .
  )" ;


  rdf::Graph g ;

  g.parseString(turtle, rdf::Graph::Format::TURTLE) ;
//  std::cout << g.serialise(rdf::Graph::Format::TURTLE) << std::endl ;

  std::string recording = "http://demo.biosignalml.org/physiobank/mitdb/102" ;

  bsml::Recording rec(recording) ;

  rec.addMetadata(g) ;


  std::cout << rec.label() << rec.format() << rec.subject() << std::endl ;

  }




