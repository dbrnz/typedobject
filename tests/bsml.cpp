#include "model.h"

#include <iostream>
#include <string>


int main(int argc, char *argv[])
/*----------------------------*/
{

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " TURTLE_FILE RECORDING_URI" << std::endl ;
    exit(1) ;
    }

  std::string source(argv[1]) ; 
  rdf::Graph g ;
  g.parseResource(source, rdf::Graph::Format::TURTLE) ;
  std::cout << g.serialise(rdf::Graph::Format::TURTLE) << std::endl ;

  std::string recording(argv[2]) ;
  bsml::Recording rec(recording) ;

  rec.addMetadata(g) ;


  std::cout << "Label: "        << rec.label()  << std::endl
            << "Format: "       << rec.format() << std::endl
            << "Investigator: " << rec.investigation() << std::endl
            << "Timeline: "     << rec.timeline().uri() << std::endl ;

  }
