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
#include "genconcatrow.h"

#define RCONC(Type) {\
Type *X1;\
Type *X2;\
    X1=(Type *)x1;\
    --X1;\
    X1 +=(*m1)*(*n1);\
    X2=(Type *)x2;\
    --X2;\
       for (i = 1; i <= (*m2)*(*n2); ++i) \
	 X1[i] = X2[i];}


int C2F(genconcatrow)(integer *typ,integer *x1,integer *m1,integer *n1,integer *x2,integer *m2,integer *n2)
{
  static integer i;
  switch (*typ) {
  case 1:
    RCONC(integer1);
    break;
  case 2:
    RCONC(integer2);
    break;
  case 4:
    RCONC(integer);
    break;
  case 11:
    RCONC(unsigned char);
    break;
  case 12:
    RCONC(unsigned short);
    break;
  case 14:
    RCONC(unsigned int);
    break;
  }
  return 0;
}
