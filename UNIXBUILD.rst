
Building pre-requisites under Unix
==================================

Tested under Ubuntu 14.04.


serd
----

::

  curl http://download.drobilla.net/serd-0.22.0.tar.bz2 | tar xj
  cd serd-0.22.0

Edit `serd.pc.in` and replace the two lines: ::

  Libs: -L${libdir} -l@LIB_SERD@
  Libs.private: -lm

with: ::

  Libs: -L${libdir} -l@LIB_SERD@ -lm

This ensures that builds using the static library also link in the math's
library. Without this change the build of `sord` (below) will fail as `-lm`
appears before `-lserd-0` on the linker's command line.

::

  export CFLAGS=-fPIC
  ./waf configure --static --no-shared
  ./waf
  sudo ./waf install


sord
----

::

  curl http://download.drobilla.net/sord-0.14.0.tar.bz2 | tar xj
  cd sord-0.14.0
  export CFLAGS=-fPIC
  ./waf configure --static --no-shared
  ./waf
  sudo ./waf install


raptor2
-------

  ./configure CFLAGS=-fPIC --disable-shared --enable-static --enable-parsers=rdfxml --enable-serializers=rdfxml

Needs statically linked curl and xml2

curl
----

  ./configure CFLAGS=-fPIC --disable-shared --enable-static


libxml2
-------

  ./configure CFLAGS=-fPIC --disable-shared --enable-static



Boost
-----

::

  cd ~/build/boost_1_59_0
  ./bootstrap.sh --with-libraries=date_time,filesystem,system,test
  ./b2 stage threading=multi link=static cxxflags=-fPIC address-model=64
  sudo ./b2 install

serd/sord
---------

* Edit `serd.pc.in` to add `-lm` to libraries line and remove `Libs.private`.
* Edit `wscript` files and add `cflags = ['-fPIC'],` to the static build options.

::

  ./waf configure --static --no-shared
  ./waf
  sudo ./waf install

