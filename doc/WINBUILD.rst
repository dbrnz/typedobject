Pre-requisites to Building
==========================

The following notes describe how to set up a development environment
for building ``typedobject``. Commands are shown to build release versions
of libraries that are suitable for linking with a release version of
``typedobject``; due to limitations of statically linking Microsoft's runtime
libraries, a debug build of ``typedobject`` requires debug versions of
libraries.

These notes install libraries into a Unix-like directory structure under
``C:\usr\local``; we first make these directories:

* Create the directories ``C:\usr``, ``C:\\usr\local`` and ``C:\usr\local\bin``.

* Add ``C:\usr\local\bin`` to the PATH.


Visual C
--------

The appropriate Visual C environment needs to be first setup by running
``vcvarsall.bat``. For 64-bit compiles under VS2013 the command is: ::

  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64 


CMake
-----

* Install CMake from http://www.cmake.org/files/v3.2/cmake-3.2.2-win32-x86.exe


PkgConfig
---------

The PkgConfig utility is used to simplify package management, both during
installation and within CMake scripts.

* Get the latest version from http://sourceforge.net/projects/pkgconfiglite/files

* Unzip into a work directory and copy the resulting ``.\bin\pkg-config.exe``
  into ``C:\usr\local\bin``.


Python
-------

Python 2.7 is required to run some build scripts and for the typedobject code
generation utility.

* Get and run the Windows installer for the latest 2.7 release
  (https://www.python.org/ftp/python/2.7.9/python-2.7.9.msi).

* Extend the PATH by adding ``c:\Python27;c:\Python27\Scripts``.


zlib
----

* Get and unzip http://zlib.net/zlib128.zip into a work directory.

* Make a build directory in the resulting ``zlib-1.2.8`` directory and change
  into it.

* Build and install a release version: ::

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=c:/usr/local -G "NMake Makefiles" ..
    nmake
    nmake install

* For a debug version of the library use ``-DCMAKE_BUILD_TYPE=Debug`` when running ``cmake``.


libiconv
--------

* Get and extract http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz into a
  work directory.

* From the typedobject git repository copy all files in the
  ``./external/VS2013/libiconv-1.14/`` directory into the new ``libiconv-1.14``
  directory. These are based on the instructions at
  http://www.codeproject.com/Articles/302012/How-to-Build-libiconv-with-Microsoft-Visual-Studio
  for building libiconv under Visual Studio.

* Make a build directory under ``libiconv-1.14`` and change into it.

* Build and install a release version: ::

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=c:/usr/local -G "NMake Makefiles" ..
    nmake
    nmake install

* For a debug version of the library use ``-DCMAKE_BUILD_TYPE=Debug`` when running ``cmake``.


libxml2
-------

* Get and extract ftp://xmlsoft.org/libxml2/libxml2-2.9.2.tar.gz into a work
  directory.

* From the installed directory: ::

    cd win32
    edit configure.js

  and change line 17 to read: ::

    var configFile = srcDirXml + "\\configure.ac";

* Now run: ::

    cscript configure.js compiler=msvc zlib=yes static=yes                        ^
      include=c:\usr\local\include\iconv lib=c:\usr\local\lib prefix=c:\usr\local
    nmake
    nmake install

* For a debug version of the library add ``debug=yes`` to the invocation of ``cscript``.


libxslt
-------
  
* Get and extract ftp://xmlsoft.org/libxslt/libxslt-1.1.28.tar.gz into a work
  directory.

* From the installed directory: ::

    cd win32
    edit Makefile.msvc

  and change line 74 to read: ::
  
    LDFLAGS = $(LDFLAGS)

* Now run: ::

    cscript configure.js compiler=msvc zlib=yes static=yes             ^
      include=c:\usr\local\include\libxml2;c:\usr\local\include\iconv  ^
      lib=c:\usr\local\lib prefix=c:\usr\local
    nmake
    nmake install

* For a debug version of the library add ``debug=yes`` to the invocation of ``cscript``.


raptor2
-------

* Get and extract http://download.librdf.org/source/raptor2-2.0.15.tar.gz into a
  work directory.

* Edit ``CMakeLists.txt`` and after line 32 add: ::

    find_package(PkgConfig)
    pkg_check_modules(ICONV iconv)

  and also add: ::

          ${ICONV_INCLUDE_DIRS}

  into the following list of include directories.

* Edit ``src\CMakeLists.txt`` and somewhere after line 118 add: ::

          raptor_escaped.c
          raptor_ntriples.c
          sort_r.c

  to the list of sources for the ``raptor2`` library.

* Edit ``src\raptor_internal.h`` and add a guard around the definition of
  __FUNCTION__ on line 81, so it reads: ::

    #ifndef __FUNCTION__
    #define __FUNCTION__ "???"
    #endif

* Edit ``src\turtle_common.c`` and after line 43 add: ::

   #define YY_NO_UNISTD_H 1

* Make a sub-directory for building the Windows version (say called
  ``winbuild``), change into it, and run: ::

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=c:/usr/local -G "NMake Makefiles" ..
    nmake
    nmake install

* For a debug version of the library use ``-DCMAKE_BUILD_TYPE=Debug`` when running ``cmake``.


pcre
----

* Download and extract the latest version from
  http://sourceforge.net/projects/pcre/files/pcre/ into a work directory.

* From the installed directory: ::

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=c:/usr/local -G "NMake Makefiles" ..
    nmake
    nmake install

* For a debug version of the library use ``-DCMAKE_BUILD_TYPE=Debug`` when running ``cmake``.


serd
----

* Download and extract the latest version from
  http://drobilla.net/software/serd/ into a work directory.

* The configuration process doesn't detect that VS2013 provides ``fmax()``. As a
  workaround, edit ``src/serd_internal.h`` and insert the following three lines
  before the line that checks ``HAVE_FMAX`` (line 41 for version 0.20.0): ::

    #if _MSC_VER == 1800    // VS2013
    # define HAVE_FMAX
    #endif

* In the top-level serd directory run: ::

    python waf configure --static --no-shared --prefix=c:\usr\local
    python waf
    python waf install

* For a debug version of the library add ``--debug`` when running
  ``python waf configure``.


sord
----

* Download and extract the latest version from
  http://drobilla.net/software/sord/ into a work directory.

* In the top-level sord directory run: ::

    python waf configure --static --no-shared --prefix=c:\usr\local
    python waf
    python waf install

* For a debug version of the library add ``--debug`` when running
  ``python waf configure``.


Boost Date Time
---------------

* Download and extract the latest version of Boost from
  http://www.boost.org/users/history into drive ``C:\``.

* Change to the installed directory and run ``bootstrap.bat``
  to build Boost's build tools.

* Now build the date-time libraries with: ::

    b2 --with-date_time variant=release link=static threading=multi toolset=msvc address-model=64

* For a debug version of the library use ``variant=debug`` when running ``b2``.

* Windows: BOOST_ROOT=C:\boost_1_58_0


libclang
--------

The typedobject code generation utility uses ``libclang`` to parse C++ header
files.

* Install LLVM and libclang by running
  http://llvm.org/releases/3.6.0/LLVM-3.6.0-win32.exe

* Add ``C:\Program Files (x86)\LLVM\bin`` to the PATH.

* Run ``pip install clang`` to install the Python bindings to libclang (``pip``
  is included with Python 2.7.9 and later releases).

