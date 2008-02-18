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
#include "i_nextj.h"
#include "stack-c.h"

extern integer C2F(memused)(integer  *it,integer *n);

#define GETCOL(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    B=(Type *)b;\
    if (m==-1) { \
      B[0]=A[0]+(j-1)*A[1];\
      r=B[0]<=A[2];\
    }\
    else { \
      r = (j<=n)&&(j>0);\
      if (r) {\
        for (i=0;i<m;i++) {\
          B[i]=A[i+(j-1)*m]; \
        }\
      }\
    }\
}

int gengetcol(integer typ, integer j,integer m,integer n,integer *a,integer *b)
{
  int r=0,i;
  switch (typ) {
  case 1:
    GETCOL(integer1);
    break;
  case 2:
    GETCOL(integer2);
    break;
  case 4:
    GETCOL(integer);
    break;
  case 11:
    GETCOL(unsigned char);
    break;
  case 12:
    GETCOL(unsigned short);
    break;
  case 14:
    GETCOL(unsigned int);
    break;
  }
  return r;
}


int C2F(inextj)(integer *j)
{
  integer il,m,n,it,l,ilr,lr,r;
  integer one=1;
  il = iadr(*Lstk(Top-1));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  m = *istk(il + 1);
  n = *istk(il + 2);
  it = *istk(il + 3);
  l = il + 4;
  
  ilr = iadr(*Lstk(Top));
  *istk(ilr) = 8;
  *istk(ilr + 2) = 1;
  *istk(ilr + 3) = it;
  lr=ilr + 4;
  if (m==-3) {/*implicit vector*/
    *istk(ilr + 1) = 1;
    *Lstk(Top+1)=sadr(lr+C2F(memused)(&it,&one));
    r=gengetcol(it,*j,-1,1,istk(l),istk(lr));
  }
  else {
    *istk(ilr + 1) = m;
    *Lstk(Top+1)=sadr(lr+C2F(memused)(&it,&m));
    r=gengetcol(it,*j,m,n,istk(l),istk(lr));
  }
  return r;
}


