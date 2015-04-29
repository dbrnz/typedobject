#ifndef TYPEDOBJECT_GLOBALS_H
#define TYPEDOBJECT_GLOBALS_H

#ifdef _WIN32
  #ifdef TYPEDOBJECT_STATIC
    #define TYPEDOBJECT_EXPORT
  #else
    #ifdef TYPEDOBJECT_BUILD
      #define TYPEDOBJECT_EXPORT __declspec(dllexport)
    #else
      #define TYPEDOBJECT_EXPORT __declspec(dllimport)
    #endif
  #endif
#else
  #define TYPEDOBJECT_EXPORT __attribute__ ((visibility ("default")))
#endif

#endif
