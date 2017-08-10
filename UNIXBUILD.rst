
Building pre-requisites under Unix
==================================

Tested under Ubuntu 14.04.


serd
----

::

  curl http://download.drobilla.net/serd-0.28.0.tar.bz2 | tar xj
  cd serd-0.28.0

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
  # Or edit `wscript` files and add `cflags = ['-fPIC'],` to the static build options.
  ./waf configure --static --no-shared
  ./waf
  sudo ./waf install


sord
----

::

  curl http://download.drobilla.net/sord-0.16.0.tar.bz2 | tar xj
  cd sord-0.16.0
  export CFLAGS=-fPIC
  # Or edit `wscript` files and add `cflags = ['-fPIC'],` to the static build options.
  ./waf configure --static --no-shared
  ./waf
  sudo ./waf install


curl
----

::

  wget https://curl.haxx.se/download/curl-7.55.0.tar.gz
  tar xzf curl-7.55.0.tar.gz
  cd curl-7.55.0
  ./configure CFLAGS=-fPIC --disable-shared --enable-static
  make
  sudo make install


libxml2
-------

::

  curl ftp://xmlsoft.org/libxml2/libxml2-2.9.4.tar.gz | tar xz
  cd libxml2-2.9.4
  ./configure CFLAGS=-fPIC --disable-shared --enable-static --without-python
  make
  sudo make install


raptor2
-------

Needs statically linked curl and xml2

::

  curl http://download.librdf.org/source/raptor2-2.0.15.tar.gz | tar xz
  cd raptor2-2.0.15
  ./configure CFLAGS=-fPIC --disable-shared --enable-static --enable-parsers=rdfxml --enable-serializers=rdfxml
  make
  sudo make install


Boost
-----

::

  wget https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.bz2
  tar xjf boost_1_64_0.tar.bz2
  cd ~/build/boost_1_64_0
  ./bootstrap.sh --with-libraries=date_time,filesystem,system,test
  ./b2 stage threading=multi link=static cxxflags=-fPIC address-model=64
  sudo ./b2 install


clang
-----

::

  curl http://releases.llvm.org/4.0.0/clang+llvm-4.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz | tar xJ
  sudo cp clang+llvm-4.0.0-x86_64-linux-gnu-ubuntu-16.04/lib/libclang.so /usr/local/lib/libclang.so.4
  sudo chmod +x /usr/local/lib/libclang.so.4
  sudo ln -s /usr/local/lib/libclang.so.4 /usr/local/lib/libclang.so
  sudo ldconfig
  #
  sudo pip install clang
