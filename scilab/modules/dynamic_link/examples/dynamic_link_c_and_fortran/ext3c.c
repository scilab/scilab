/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



/************************************
 *     simple example 3 (passing a chain) 
 * -->host('make /tmp/ext3c.o');
 * -->link('/tmp/ext3c.o','ext3c','C');
 * -->a=[1,2,3];b=[4,5,6];n=3;
 * -->c=call('ext3c','yes',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d') 
 * -->c-(sin(a)+cos(b) )
 * -->c=call('ext3c','no',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d') 
 * -->c-(a+b )
 ************************************/

#include <math.h>
#include <string.h>

int ext3c(char *ch, int *n, double *a, double *b, double *c)
{
  static int k;
  if (strcmp(ch, "yes") == 0) 
    {
      for (k = 0; k < *n; ++k) 
	c[k] = sin(a[k]) + cos(b[k]);
    } 
  else 
    {
      for (k = 0; k < *n; ++k) 
	c[k] = a[k] + b[k];
    }
  return(0);
} 
