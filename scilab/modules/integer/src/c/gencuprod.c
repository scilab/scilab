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
#include "gencuprod.h"

#define CUPROD(Type) {\
Type *W;\
    W=(Type *)w;\
    t = 1; kk = 0;\
    for (k = 0; k < *n; ++k) {\
        t *= (integer)W[kk];\
	W[kk] =(Type)t;kk += (*iw);\
    }\
}


int C2F(gencuprod)(integer *typ,integer *n, void *w, integer *iw)
{
  integer k, kk, t;

  switch (*typ) {
  case 1:
    CUPROD(integer1);
    break;
  case 2:
    CUPROD(integer2);
    break;
  case 4:
    CUPROD(integer);
    break;
  case 11:
    CUPROD(unsigned char);
    break;
  case 12:
    CUPROD(unsigned short);
    break;
  case 14:
    CUPROD(unsigned int);
    break;
  }
  return 0;
}

