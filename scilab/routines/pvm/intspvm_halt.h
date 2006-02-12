/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTSPVM_HALT__
#define __INTSPVM_HALT__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

#include <string.h> 
#ifdef WIN32
  #include "../../pvm3/include/pvm3.h"
#else
  #include "pvm3.h"
#endif

#include "../stack-c.h"
#include "sci_pvm.h"

int intspvm_halt(char *fname);

#endif /* __INTSPVM_HALT__ */
/*-----------------------------------------------------------------------------------*/ 
