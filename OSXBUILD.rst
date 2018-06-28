Building under OS/X
===================

Boost
-----

* Unlink any version of Boost installed by Homebrew: ::

  $ brew unlink boost

* Download and extract the latest version of Boost from
  http://www.boost.org/users/history. Then change into
  the installation directory and: ::

  $ ./bootstrap.sh --with-libraries=date_time,filesystem,system,test
  $ ./b2 stage threading=multi link=static cxxflags=-fPIC address-model=64
  $ ./b2 install


Python 3 and libclang
---------------------

::

  $ brew install llvm --with-clang
  $ pip install clang

Ensure `/usr/local/libclang.dylib` is linked to that installed by Homebrew
(in `/usr/local/Cellar/llvm/VERSION/lib`); create the link if necessary.
