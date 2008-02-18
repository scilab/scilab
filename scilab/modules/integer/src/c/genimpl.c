/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "machine.h"
#include "genimpl.h"

#define IMPL2(Type) {\
Type FIRST = (Type)*((Type *)first);\
Type LAST  = (Type)*((Type *)last);\
Type *RES  = (Type *)res;\
Type j;\
    i = 0;\
    for (j=FIRST;j<=LAST;j++){\
      RES[i++]=j;\
    }\
}

#define IMPL3(Type) {\
Type FIRST = (Type)*((Type *)first);\
Type STEP  = (Type)*((Type *)step);\
Type LAST  = (Type)*((Type *)last);\
Type *RES  = (Type *)res;\
Type j;\
    FIRST=(Type)*((Type *)first);\
    LAST=(Type)*((Type *)last);\
    STEP=(Type)*((Type *)step);\
    RES=(Type *)res;\
    i = 0;\
    if (STEP<(Type)0) {\
      for (j=FIRST;j>=LAST;j+=STEP){\
        RES[i++]=j;\
      }\
    }\
    else if (STEP>(Type)0) {\
      for (j=FIRST;j<=LAST;j+=STEP){\
        RES[i++]=j;\
      }\
    }\
}

#define IMPL2DIM(Type) {\
Type FIRST = (Type)*((Type *)first);\
Type LAST  = (Type)*((Type *)last);\
     *res = (integer)(LAST-FIRST)+1;\
}
#define IMPL3DIM(Type) {\
Type FIRST = (Type)*((Type *)first);\
Type STEP  = (Type)*((Type *)step);\
Type LAST  = (Type)*((Type *)last);\
Type j;\
   i = 0;\
   if (STEP<(Type)0) {\
     for (j=FIRST;j>=LAST;j=j+STEP) {i++;}\
   }\
   else if (STEP>(Type)0) {\
     for (j=FIRST;j<=LAST;j=j+STEP) {i++;}\
   }\
   else {\
    i=-1;\
   }\
   *res = i;\
}

int C2F(genimpl2)(integer *typ,integer *first,integer *last,integer *res)
{
  static int i;

  switch (*typ) {
  case 1:
    IMPL2(integer1);
    break;
  case 2:
    IMPL2(integer2);
    break;
  case 4:
    IMPL2(integer);
    break;
  case 11:
    IMPL2(unsigned char);
    break;
  case 12:
    IMPL2(unsigned short);
    break;
  case 14:
    IMPL2(unsigned int);
    break;
  }
  return 0;
}

int C2F(genimpl3)(integer *typ,integer *first,integer *step,integer *last,integer *res)
{
  static int i;

  switch (*typ) {
  case 1:
    IMPL3(integer1);
    break;
  case 2:
    IMPL3(integer2);
    break;
  case 4:
    IMPL3(integer);
    break;
  case 11:
    IMPL3(unsigned char);
    break;
  case 12:
    IMPL3(unsigned short);
    break;
  case 14:
    IMPL3(unsigned int);
    break;
  }
  return 0;
}

int C2F(genimpl2dim)(integer *typ,integer *first,integer *last,integer *res)
{

  switch (*typ) {
  case 1:
    IMPL2DIM(integer1);
    break;
  case 2:
    IMPL2DIM(integer2);
    break;
  case 4:
    IMPL2DIM(integer);
    break;
  case 11:
    IMPL2DIM(unsigned char);
    break;
  case 12:
    IMPL2DIM(unsigned short);
    break;
  case 14:
    IMPL2DIM(unsigned int);
    break;
  }
  return 0;
}
int C2F(genimpl3dim)(integer * typ,integer * first,integer * step,integer * last,integer * res)
{
  static int i;

  switch (*typ) {
  case 1:
    IMPL3DIM(integer1);
    break;
  case 2:
    IMPL3DIM(integer2);
    break;
  case 4:
    IMPL3DIM(integer);
    break;
  case 11:
    IMPL3DIM(unsigned char);
    break;
  case 12:
    IMPL3DIM(unsigned short);
    break;
  case 14:
    IMPL3DIM(unsigned int);
    break;
  }
  return 0;
}


