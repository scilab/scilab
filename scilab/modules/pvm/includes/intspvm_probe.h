/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTSPVM_PROBE__
#define __INTSPVM_PROBE__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

#include <string.h> 
#ifdef _MSC_VER
  #include "../src/pvm3/include/pvm3.h"
#else
  #include "pvm3.h"
#endif

#include "stack-c.h"
#include "../src/sci_pvm.h"

int intspvm_probe(char *fname);

#endif /* __INTSPVM_PROBE__ */
/*-----------------------------------------------------------------------------------*/ 
