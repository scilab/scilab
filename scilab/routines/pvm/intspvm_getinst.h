/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTSPVM_GETINST__
#define __INTSPVM_GETINST__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

#include <string.h> 
#ifdef _MSC_VER
  #include "../../pvm3/include/pvm3.h"
#else
  #include "pvm3.h"
#endif

#include "../stack-c.h"
#include "sci_pvm.h"

int intspvm_getinst(char *fname);

#endif /* __INTSPVM_GETINST__ */
/*-----------------------------------------------------------------------------------*/ 
