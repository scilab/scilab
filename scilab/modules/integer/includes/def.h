#ifndef __DEF_H__
#define __DEF_H__

#ifdef integer
#undef integer
typedef long int /* long int */ integer;
#endif

typedef double doublereal;

#ifdef doublecomplex
#undef doublecomplex
typedef struct { doublereal r, i; } doublecomplex;
#endif


typedef unsigned long int /* long */ uinteger;
typedef char *address;
typedef short int shortint;
typedef float real;

typedef struct { real r, i; } complex;
typedef long int /* long int */ logical;
typedef short int shortlogical;
typedef char logical1;
typedef signed char integer1;
typedef short integer2;

#endif /*__DEF_H__*/

