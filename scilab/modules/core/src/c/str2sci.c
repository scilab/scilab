/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <string.h>
#include "stack-c.h"
#include "str2sci.h"
#include "cvstr.h"
#include "do_error_number.h"

void str2sci(char** x,int n,int m)
{
      
  int l=0,il=0,zero=0,err,n1,i,m1=0;
      
  if (Top >= Bot) {
    i=18;
    SciError(i);
  } else {
    Top = Top + 1;
    il = iadr(*Lstk(Top));
    l = sadr(il+6);
  }
  
  err = l + n*m - *Lstk(Bot);
  if (err > 0) {
    i=17;
    SciError(i);
    return;
  }
  *istk(il) = sci_strings;
  *istk(il+1) = n;
  *istk(il+2) = m;
  *istk(il+3) = 0;
  *istk(il+4) = 1;
  for (i = 1; i <= n*m; i++){
	  n1=(int)strlen(x[i-1]);
	  *istk(il+4+i) =  *istk(il+4+i-1)+n1;
	  if (n1 > 0) 
		  C2F(cvstr)(&n1,istk(il+m*n+5 -1 + *istk(il+3+i)),x[i-1],&zero,(unsigned long) n1);
	  m1=m1+n1;
  }
  *Lstk(Top+1) = l + m1;      
                
} 
      
