#ifndef DUST_GLOBAL_H
#define DUST_GLOBAL_H

#if defined(DUST_LIBRARY)
#  define DUSTSHARED_EXPORT __declspec(dllexport)
#else
#  define DUSTSHARED_EXPORT __declspec(dllimport)
#endif

#endif // DUST_GLOBAL_H
