
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
