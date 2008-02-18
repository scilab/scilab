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
#include "gencusum.h"
#define CUSUM(Type) {\
Type *W;\
    W=(Type *)w;\
    t = 0; kk = 0;\
    for (k = 0; k < *n; ++k) {\
        t += (integer)W[kk];\
	W[kk] =(Type)t;kk += (*iw);\
    }\
}


int C2F(gencusum)(integer *typ,integer *n, integer *w, integer *iw)
{
  integer k, kk, t;

  switch (*typ) {
  case 1:
    CUSUM(integer1);
    break;
  case 2:
    CUSUM(integer2);
    break;
  case 4:
    CUSUM(integer);
    break;
  case 11:
    CUSUM(unsigned char);
    break;
  case 12:
    CUSUM(unsigned short);
    break;
  case 14:
    CUSUM(unsigned int);
    break;
  }
  return 0;
}

