typedobject
===========

``typedobject`` is a C++ library and specification framework for RDF processing.
It was developed for implementating the BioSignalML C++ library, but as a
separate project, with the intent that others may find it useful. The code builds
and runs under Windows, Linux and OS/X.

As well as a providing a multi-platform RDF library, ``typedobject`` enables the
relationships between RDF statements about a class of resources and the C++ class
that represents the resource to be specified in a succinct way. This is done by
embeddingn a DSL into C++ class declarations (via macros) and using pre-processing
to generate C++ implementation code.


A simple example
----------------

Given the following RDF graph (in Turtle, and assumed to be at
http://example.org/rdf/statements): ::

  http:://example.org/object#1
    a owl:Object ;
    rdfs:label "A label" ;
    dct:description "Some long description" .

then the following will retrieve values of properties::

  #include <iostream>
  #include <typedobject/typedobject.h>

  class Object : public TypedObject::TypedObject
  /*------------------------------------------*/
  {
    TYPED_OBJECT(Object, OWL::Object)

    PROPERTY_STRING(label, RDFS::label)
    PROPERTY_STRING(comment, RDFS::comment)
    PROPERTY_STRING(description, DCT::description)
    PROPERTY_NODE(precededBy, PRV::precededBy)
    PROPERTY_URI(creator, DCT::creator)
    PROPERTY_DATETIME(created, DCT::created)
    } ;

  int main(void)
  {
    rdf::Graph graph ;
    graph.parse_resource("http://example.org/rdf/statements", rdf::Format::TURTLE) ;
    Object obj("http://example.org/object#1", graph) ;
    std::cout << obj.label() << std::endl ;   // Outputs "A label"
    }



Requirements
------------

* sord (http://drobilla.net/software/sord/) for storage.
* serd (http://drobilla.net/software/serd/) for Turtle and NTriples parsing and
  serialisation.
* Raptor (http://librdf.org/raptor/) for RDF/XML parsing and serialisation.
* CMake is used for builds.

Platform specific notes are available for `Windows <./WINBUILD.rst>`_ and
`OS/X <./OSXBUILD.rst>`_.


Code generator:
~~~~~~~~~~~~~~~

* libclang 3.6.2 -- earlier versions simply don't work...
* Python 2.7
* Python bindings for libclang -- `pip install clang`



History
-------

The motivation for this work has been the need for a cross-platform RDF library
to use in a C++ implementation of BioSignalML. (BioSignalML is a RDF model for
describing physiological signals, developed as part of the Physiome Project.)

To date, Python has been used as the implementation language for BioSignalML,
with RDF processing at first being done using the Redland RDF library (via its
Python bindings) and then, because Redland is not supported under Windows,
with ``rdflib``, a pure Python package for RDF.

With the need to provide a BioSignalML plugin for OpenCOR (http://opencor.ws/)
we considered embedding a Python interpreter into OpenCOR. This though ran
into issues under Windows (due to different versions of Visual Studio used for
standard Python ond OpenCOR), hence the decison to develop a C++ version of
BioSignalML and the need for a cross-platform RDF library that could be easily
built under Windows.
  

Other C/C++ RDF libraries
-------------------------

* Redland RDF libraries (http://librdf.org/) -- even though one component, the
  Raptor RDF syntax library, does build under Windows, the full Redland library is
  unsupported.
* DataQuay (https://bitbucket.org/breakfastquay/dataquay/) -- Qt4 specific.
* sord (http://drobilla.net/software/sord/) and serd
  (http://drobilla.net/software/serd/), which we make use of.

