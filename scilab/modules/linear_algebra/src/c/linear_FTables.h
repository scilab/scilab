/**************************************
 * Sylvestre Ledru - INRIA 2007
 **************************************/

#include "machine.h"

#define OK 1
#define FAIL 0

typedef void (*voidf)();

typedef struct { double r, i; } doublecmplx;

typedef struct {
  char *name;
  voidf f;
} FTAB;
