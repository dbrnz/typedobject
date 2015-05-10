prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_PREFIX}
libdir=${CMAKE_INSTALL_PREFIX}/lib
includedir=${CMAKE_INSTALL_PREFIX}/include

Name: libiconv
Description: Convert between character encodings
Version: 1.14.0

Requires:
Libs: -L${CMAKE_INSTALL_PREFIX}/lib -llibiconv
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/iconv
