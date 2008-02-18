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
#include "genextrac1.h"

#define EXTRAC1(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       for (i = 1; i <= *mi; ++i) \
	 TO[i] = FROM[indx[i]];\
}



int C2F(genextrac1)(integer *typ,integer *mi,integer *indx,integer *from,integer *to)
{
  static integer i;
  --indx;
  switch (*typ) {
  case 1:
    EXTRAC1(integer1);
    break;
  case 2:
    EXTRAC1(integer2);
    break;
  case 4:
    EXTRAC1(integer);
    break;
  case 11:
    EXTRAC1(unsigned char);
    break;
  case 12:
    EXTRAC1(unsigned short);
    break;
  case 14:
    EXTRAC1(unsigned int);
    break;
  }
  return 0;
}

