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
#include "geninsert1.h"

#define INSERT1(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       for (i = 1; i <= *mi; ++i) \
	 TO[indx[i]] = FROM[1+(i-1)*(*inc2)];\
}

int C2F(geninsert1)(integer *typ,integer *mi,integer *indx,integer *to,integer *from,integer *inc2)
{

  static integer i;
  --indx;
  switch (*typ) {
  case 1:
    INSERT1(integer1);
    break;
  case 2:
    INSERT1(integer2);
    break;
  case 4:
    INSERT1(integer);
    break;
  case 11:
    INSERT1(unsigned char);
    break;
  case 12:
    INSERT1(unsigned short);
    break;
  case 14:
    INSERT1(unsigned int);
    break;
  }
  return 0;
}

