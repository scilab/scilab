/*  Scicos
*
*  Copyright (C) INRIA - Allan LAYEC
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/

/* TODO SPLIT THIS FILE !!!! */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "scicos_block4.h"
#include "sciblk2.h"
#include "scicos.h"
#include "intcscicos.h"
#include "Scierror.h"
#include "cvstr.h"
#include "sciblk4.h"
/*--------------------------------------------------------------------------*/
extern int C2F(mkmlist)();
extern void str2sci(char** x,int n,int m);
extern int *listentry(int *header, int i);
extern int CopyVarFromlistentry(int lw, int *header, int i);
extern int createblklist(scicos_block *Blocks, int *ierr, int flag_imp, int funtyp);
extern int C2F(funnum) (char *fname);
extern int C2F(namstr)();
extern void  GetDynFunc();
extern void  F2C(sciblk)();

extern int ntabsim;
extern OpTab tabsim[];
/*--------------------------------------------------------------------------*/
int extractblklist(int *il, scicos_block *Blocks, int *ierr);
/*--------------------------------------------------------------------------*/
/*
 * int int intdata2sig(fname,fname_len)
 * int int intsig2data(fname,fname_len)
 * int intmodel2blk(fname,fname_len)
 * int intcallblk(fname,fname_len)
 * int extractblklist(int *il, scicos_block *Block, int *ierr)
 */

/* B=data2sig(A[,step])
 *
 * Put data in a 'scicos' signal structure.
 * If A is a vector, then it will be understand
 * as a scalar that evolves along time.
 * If A is a matrix, then it will be understand
 * as a vector that evolves along time.
 * If A is an hypermatrix, then it will be understand
 * as a matrix that evolves along time.
 *
 * Usage :
 * -->a(:,:,4)=([7 8 9;10 11 12]);
 * -->b=data2sig(a,0.1)
 * b  =
 *   values: [2x3x4 hypermat]
 *   time: [4x1 constant]
 * -->b.time
 * ans  =
 *    0.
 *    0.1.
 *    0.2.
 *    0.3.
 *
 * Inputs
 *   A    : vector,matrix or hypermatrix(with 3 dimensions)
 *          of real/complex or integers numbers,
 *
 *   step : an optional argument that gives the time step
 *          for the output time field.
 *          If not given, then the time step is 1.
 *
 * Output
 *   B    : a scilab structure that defines a signal :
 *          B.values :  A input,
 *          B.time   :  the samples time in the form of
 *                      a columun vector. The size of that
 *                      vector is given by the A input.
 *
 * initial rev 08/11/07, Alan
 *
 */
int sci_data2sig(char *fname, unsigned long fname_len)
{
  int m1 = 0,n1 = 0,it = 0;
  int p1 = 0;
  int *il1 = NULL;
  int m1h = 0,n1h = 0;
  int *ilh1 = NULL;
  int m2h = 0,n2h = 0;
  int *ilh2=NULL;
  int *ilh3=NULL;
  int m2 = 0,n2 = 0,it2 = 0;
  int *il2 = NULL;
  int il = 0, l = 0;
  int l_tmp = 0;

  int i = 0, j = 0, TopSave = 0;

  char *str_stlst[]={"st","dims","values","time"};
  char *str_hmlst[]={"hm","dims","entries"};

  double step_time=1;

  int  ismat=0;

  CheckRhs(1,2);
  CheckLhs(1,1);

  il1 = (int *) GetData(1);
  m1  = il1[1];
  n1  = il1[2];
  it  = il1[3];
  if ((il1[0]!=1) && (il1[0]!=8)) {
    if (il1[0]==17) {
      ilh1 = (int *) (listentry(il1,1));
      m1h  = ilh1[1];
      n1h  = ilh1[2];
      if ((ilh1[m1h*n1h+5]!=17) || (ilh1[m1h*n1h+6]!=22)) {
        Scierror(888,"%s : First argument must be a matrix or an hypermatrix\n"
                "          of real, complex or integer numbers.\n",fname);
        return 0;
      }
      ilh2 = (int *) (listentry(il1,2));
      m2h  = ilh2[1];
      n2h  = ilh2[2];
      if (n2h!=3) {
        Scierror(888,"%s : Only hypermatrix with 3 dimensions is allowed.\n",\
                 fname);
        return 0;
      }
      m1     = ilh2[4];
      n1     = ilh2[5];
      p1     = ilh2[6];
      ilh3   = (int *) (listentry(il1,3));
      if ((ilh3[0]!=1) && (ilh3[0]!=8)) {
        Scierror(888,"%s : First argument must be a matrix or an hypermatrix\n"
                "          of real, complex or integer numbers.\n",fname);
        return 0;
      }
      it     = ilh3[3];
    }
    else {
      Scierror(888,"%s : First argument must be a matrix or an hypermatrix\n"
              "          of real, complex or integer numbers.\n",fname);
      return 0;
    }
  }

  if (Rhs==2) {
    il2 = (int *) GetData(2);
    m2  = il2[1];
    n2  = il2[2];
    it2 = il2[3];
    if ((m2*n2)!=1) {
      Scierror(888,"%s : Secong argument must be a scalar.\n",fname);
      return 0;
    }
    if ((il2[0]!=1) || (it2!=0)) {
      Scierror(888,"%s : Secong argument must be a real.\n",fname);
      return 0;
    }
    step_time = *((double *) &il2[4]);
  }

  TopSave=Top;

  str2sci(str_stlst,1,4);

  Top           = Top + 1;
  il            = iadr(*Lstk(Top));
  l             = sadr(il+4);
  *istk(il)     = 8;
  *istk(il+1)   = 1;
  *istk(il+2)   = 2;
  *istk(il+3)   = 4;
  *istk(il+4)   = 1;
  *istk(il+5)   = 1;
  *Lstk(Top+1)  = l + 1;

  if ((m1!=1) && (n1!=1)) {
    ismat=1;
  }

  switch(il1[0])
  {
   case 1 :
           {
            Top            = Top + 1;
            il             = iadr(*Lstk(Top));
            l              = sadr(il+4);
            *istk(il)      = 1;
            if (ismat) {
              *istk(il+1)  = m1;
              *istk(il+2)  = n1;
            }
            else {
              *istk(il+1)  = n1*m1;
              *istk(il+2)  = 1;
            }
            *istk(il+3)    = it;
            if (it==0) {
              for(i=0;i<(m1*n1);i++) {
                stk(l)[i]  = *((double *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + n1*m1;
            }
            else if (it==1) {
              for(i=0;i<(2*m1*n1);i++) {
                stk(l)[i]  = *((double *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + 2*n1*m1;
            }
            break;
           }

   case 8 :
           {
            Top            = Top + 1;
            il             = iadr(*Lstk(Top));
            l              = sadr(il+4);
            *istk(il)      = 8;
            if (ismat) {
              *istk(il+1)  = m1;
              *istk(il+2)  = n1;
            }
            else {
              *istk(il+1)  = m1*n1;
              *istk(il+2)  = 1;
            }
            *istk(il+3)    = it;
            if (it==4) {
              for(i=0;i<(m1*n1);i++) {
                *((long *)(istk(il+4)) + i)  = *((long *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/2)+1);
            }
            else if (it==2) {
              for(i=0;i<(m1*n1);i++) {
                *((short *)(istk(il+4)) + i)  = *((short *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/4)+1);
            }
            else if (it==1) {
              for(i=0;i<(m1*n1);i++) {
                *((char *)(istk(il+4)) + i)  = *((char *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/8)+1);
            }
            else if (it==14) {
              for(i=0;i<(m1*n1);i++) {
                *((unsigned long *)(istk(il+4)) + i)  = *((unsigned long *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/2)+1);
            }
            else if (it==12) {
              for(i=0;i<(m1*n1);i++) {
                *((unsigned short *)(istk(il+4)) + i)  = *((unsigned short *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/4)+1);
            }
            else if (it==11) {
              for(i=0;i<(m1*n1);i++) {
                *((unsigned char *)(istk(il+4)) + i)  = *((unsigned char *)(&il1[4]) + i);
              }
              *Lstk(Top+1) = l + (int)(ceil((n1*m1)/8)+1);
            }
            break;
           }

   case 17 :
           {
            str2sci(str_hmlst,1,3);

            Top           = Top + 1;
            il            = iadr(*Lstk(Top));
            l             = sadr(il+4);
            *istk(il)     = 8;
            *istk(il+1)   = 1;
            *istk(il+2)   = n2h;
            *istk(il+3)   = 4;
            for(i=0;i<(n2h);i++) {
              *((long *)(istk(il+4)) + i)  = *((long *)(&ilh2[4]) + i);
            }
            *Lstk(Top+1) = l + (int)(ceil((n2h)/2)+1);

            switch(ilh3[0])
            {
             case 1 :
                     {
                      Top            = Top + 1;
                      il             = iadr(*Lstk(Top));
                      l              = sadr(il+4);
                      *istk(il)      = 1;
                      *istk(il+1)    = m1*n1*p1;
                      *istk(il+2)    = 1;
                      *istk(il+3)    = it;
                      if (it==0) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          stk(l)[i]  = *((double *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + n1*m1*p1;
                      }
                      else if (it==1) {
                        for(i=0;i<(2*m1*n1*p1);i++) {
                           stk(l)[i] = *((double *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + 2*n1*m1*p1;
                      }
                      break;
                     }
             case 8 :
                     {
                      Top            = Top + 1;
                      il             = iadr(*Lstk(Top));
                      l              = sadr(il+4);
                      *istk(il)      = 8;
                      *istk(il+1)    = m1*n1*p1;
                      *istk(il+2)    = 1;
                      *istk(il+3)    = it;
                      if (it==4) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((long *)(istk(il+4)) + i)  = *((long *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/2)+1);
                      }
                      else if (it==2) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((short *)(istk(il+4)) + i)  = *((short *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/4)+1);
                      }
                      else if (it==1) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((char *)(istk(il+4)) + i)  = *((char *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/8)+1);
                      }
                      else if (it==14) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((unsigned long *)(istk(il+4)) + i)  = *((unsigned long *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/2)+1);
                      }
                      else if (it==12) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((unsigned short *)(istk(il+4)) + i)  = *((unsigned short *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/4)+1);
                      }
                      else if (it==11) {
                        for(i=0;i<(m1*n1*p1);i++) {
                          *((unsigned char *)(istk(il+4)) + i)  = *((unsigned char *)(&ilh3[4]) + i);
                        }
                        *Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/8)+1);
                      }
                      break;
                     }
            }
            C2F(mkmlist)((i=3,&i));
            break;
           }
  }

  Top           = Top + 1;
  il            = iadr(*Lstk(Top));
  l             = sadr(il+4);
  *istk(il)     = 1;
  if (il1[0]==17) {
    *istk(il+1)   = p1;
  }
  else {
    if (ismat) {
      *istk(il+1) = m1;
    }
    else {
      *istk(il+1) = n1*m1;
    }
  }
  *istk(il+2)   = 1;
  *istk(il+3)   = 0;
  for(i=0;i<(*istk(il+1));i++) {
    stk(l)[i]  = (double) i*step_time;
  }
  *Lstk(Top+1) = l + *istk(il+1);

  C2F(mkmlist)((i=4,&i));

  Top = TopSave;

  if (Rhs==2) {
    CreateVar(3,"m",(i=4,&i),(j=1,&j),&l_tmp);
    LhsVar(1)=3;
  }
  else {
    CreateVar(2,"m",(i=4,&i),(j=1,&j),&l_tmp);
    LhsVar(1)=2;
  }

  return 0;
}
/*--------------------------------------------------------------------------*/
/* [x[,t]]=sig2data(B)
 *
 * Extract data from a 'scicos' signal structure.
 *  x will be the field B.values
 *  t will be the field B.time
 *
 * Usage :
 * -->a(:,:,4)=([7 8 9;10 11 12]);
 * -->b=data2sig(a,0.1)
 * b  =
 *   values: [2x3x4 hypermat]
 *   time: [4x1 constant]
 * -->b.time
 * ans  =
 *    0.
 *    0.1.
 *    0.2.
 *    0.3.
 *
 * -->[x,t]=sig2data(b)
 * t  =
 *  0.
 *   0.1
 *   0.2
 *   0.3
 * x  =
 * (:,:,1)
 *
 *  0.    0.    0.
 *  0.    0.    0.
 *
 * Inputs
 *   B    : a scilab structure that defines a signal :
 *          B.values :  B data,
 *          B.time   :  the samples time in the form of
 *                      a columun vector.
 *
 * Outputs
 *   x    : the field B.values
 *   t    : the field B.time
 *
 * initial rev 08/11/07, Alan
 *
 */
int sci_sig2data(char *fname, unsigned long fname_len)
{
  int m1,n1;
  int *il1;
  int m1h,n1h;
  int *ilh1;
  int ierr;

  CheckRhs(1,1);
  CheckLhs(1,2);

  il1 = (int *) GetData(1);
  m1  = il1[1];
  n1  = il1[2];

  if (il1[0]!=17) {
    Scierror(888,"%s : First argument must be a structure.\n",fname);
    return 0;
  }

  ilh1 = (int *) (listentry(il1,1));
  m1h  = ilh1[1];
  n1h  = ilh1[2];
  if ((ilh1[m1h*n1h+5]!=28) || (ilh1[m1h*n1h+6]!=29)) {
    Scierror(888,"%s : First argument must be a structure.\n",fname);
    return 0;
  }
  if ((n1h!=4) || \
      (ilh1[m1h*n1h+11]!=31) || \
      (ilh1[m1h*n1h+12]!=10) || \
      (ilh1[m1h*n1h+13]!=21) || \
      (ilh1[m1h*n1h+14]!=30) || \
      (ilh1[m1h*n1h+15]!=14) || \
      (ilh1[m1h*n1h+16]!=28) || \
      (ilh1[m1h*n1h+17]!=29) || \
      (ilh1[m1h*n1h+18]!=18) || \
      (ilh1[m1h*n1h+19]!=22) || \
      (ilh1[m1h*n1h+20]!=14)) {
    Scierror(888,"%s : First argument must be a scicos signal structure.\n",fname);
    return 0;
  }

  ierr=CopyVarFromlistentry(Top-Rhs+2,il1,3);
  LhsVar(1)=2;
  if (Lhs==2) {
    ierr=CopyVarFromlistentry(Top-Rhs+3,il1,4);
    LhsVar(2)=3;
  }

  return 0;
}
/*--------------------------------------------------------------------------*/
/* intmodel2blk Build a scicos_block structure from
 * a scicos model
 *
 * [Block] = model2blk(objs.model)
 *
 * rhs 1 : a scicos model Tlist
 *        - 1  : model(1)     : !model      sim     in      in2     intyp    out    out2  outtyp
 *                               evtin      evtout  state   dstate  odstate  rpar   ipar  opar
 *                               blocktype  firing  dep_ut  label   nzcross  nmode  equations  !
 *        - 2  : model.sim       :
 *        - 3  : model.in        :
 *        - 4  : model.in2       :
 *        - 5  : model.intyp     :
 *        - 6  : model.out       :
 *        - 7  : model.out2      :
 *        - 8  : model.outtyp    :
 *        - 9  : model.evtin     :
 *        - 10 : model.evtout    :
 *        - 11 : model.state     :
 *        - 12 : model.dsate     :
 *        - 13 : model.odsate    :
 *        - 14 : model.rpar      :
 *        - 15 : model.ipar      :
 *        - 16 : model.opar      :
 *        - 17 : model.blocktype :
 *        - 18 : model.firing    :
 *        - 19 : model.dep_ut    :
 *        - 20 : model.label     :
 *        - 21 : model.nzcross   :
 *        - 22 : model.nmode      :
 *        - 23 : model.equations :
 *
 * lhs 1 : a scicos block Tlist
 *
 * initial rev 12/11/07, Alan
 * 05/07/08, Alan : fix for xprop
 *                  add extra comments
 *                  check in/out size and type
 *
 */
int sci_model2blk(char *fname, unsigned long fname_len)
{
  int m1,n1;
  int *il1;
  int n;
  int mh,nh;
  int *ilh;
  int mh2,nh2;
  int *ilh2;
  int mh3,nh3;
  int *ilh3;
  int ierr;
  int TopSave;
  int i,j;
  int l_tmp;
  double type;
  int len_str=0;
  int lfunpt=0;
  int typfsim;
  int ifun;
  int *il_sim;

  int id[nsiz];

  double *ptr_d;
  char *ptr_c;
  unsigned char *ptr_uc;
  short *ptr_s;
  unsigned short *ptr_us;
  long *ptr_l;
  unsigned long *ptr_ul;

  scicos_block Block;

  int nblklst=40;

  /* check size of rhs/lhs parameters */
  CheckRhs(1,1);
  CheckLhs(1,1);

  il1 = (int *) GetData(1);
  m1  = il1[1];
  n1  = il1[2];

  /* check for a tlist */
  if (il1[0]!=17) {
    Scierror(888,"%s : First argument must be a Typed list.\n",fname);
    return 0;
  }

  /* check for a type "scicos model" */
  ilh = (int *) (listentry(il1,1));
  mh  = ilh[1];
  nh  = ilh[2];
  if ((ilh[mh*nh+5]!=22) || \
      (ilh[mh*nh+6]!=24) || \
      (ilh[mh*nh+7]!=13) || \
      (ilh[mh*nh+8]!=14) || \
      (ilh[mh*nh+9]!=21)) {
    Scierror(888,"%s : First argument must be a scicos model.\n",fname);
    return 0;
  }

  /* TODO */
  /* 2 : model.sim  */
  n            = MlistGetFieldNumber(il1, "sim");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  if (ilh[0]==15) { /* sim  is a list */
    ilh2 = (int *) (listentry(ilh,2));
    Block.type = (int) *((double *) (&ilh2[4]));
    ilh2 = (int *) (listentry(ilh,1));
    typfsim = ilh2[0]; /* typfsim  the name the comput funct */
    il_sim = ilh2;
  }
  else {
    Block.type = 0;
    typfsim = ilh[0];
    il_sim = ilh;
  }
  /* check if typfsim is a scilab function */
  if ((typfsim==11) || (typfsim==13)) {
   /* TODO */
   lfunpt = -1;
  }
  /* check if typfsim is a string */
  else if(typfsim==10){
    len_str = il_sim[5]-1;
    C2F(cha1).buf[0]=' ';
    C2F(cvstr)(&len_str,&il_sim[6],&C2F(cha1).buf[0],(j=1,&j),len_str);
    C2F(cha1).buf[len_str]='\0';
    /* search fun ptr of the comput func in the scilab func table */
    ifun=C2F(funnum)(C2F(cha1).buf);
    if (ifun>0) lfunpt=ifun;
    else {
      C2F(namstr)(id,&il_sim[6],&len_str,(j=0,&j));
      C2F(com).fin=0;
      C2F(funs)(id);
      if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt=-*Lstk(C2F(com).fin);
      else
      {
       lfunpt=0;
       Scierror(888,"%s : unknown block : %s\n",fname,C2F(cha1).buf);
       return 0;
      }
    }
  }
  /* else {
   * error
   }*/
  /* comput func is a scilab function */
  if (lfunpt<0) {
    switch (Block.type) {
    case 0:
      Block.funpt=F2C(sciblk);
      break;
    case 1:
      Scierror(888,"%s : type 1 function not allowed for scilab blocks\n",fname);
      return 0;
    case 2:
      Scierror(888,"%s : type 2 function not allowed for scilab blocks\n",fname);
      return 0;
    case 3:
      Block.funpt=sciblk2;
      Block.type=2;
      break;
    case 5:
      Block.funpt=sciblk4;
      Block.type=4;
      break;
    case 99: /* debugging block */
      Block.funpt=sciblk4;
      Block.type=4;
      break;
    case 10005:
      Block.funpt=sciblk4;
      Block.type=10004;
      break;
    default :
      Scierror(888,"%s : Undefined Function type\n",fname);
      return 0;
    }
    Block.scsptr=-lfunpt;
  }
  else if (lfunpt<=ntabsim) {
    Block.funpt=*(tabsim[lfunpt-1].fonc);
    Block.scsptr=0;
  }
  else {
    lfunpt -= (ntabsim+1);
    GetDynFunc(lfunpt,&Block.funpt);
    if (Block.funpt == (voidf) 0) {
      Scierror(888,"%s : Function not found\n",fname);
      return 0;
    }
    Block.scsptr=0;
  }

  /* check input ports */
  /* 3 : model.in  */
  n            = MlistGetFieldNumber(il1, "in");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nin    = mh*nh;
  Block.insz   = NULL;
  Block.inptr  = NULL;
  if (mh*nh!=0) {
    /* check value of in */
    for (i=0;i<Block.nin;i++) {
      if ((*((double *)(&ilh[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Size. in(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh[4]) + i)));
        return 0;
      }
    }
    /* alloc insz */
    if ((Block.insz=(int *) MALLOC(Block.nin*3*sizeof(int)))==NULL) {
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    /* alloc inptr */
    if ((Block.inptr=(void **) MALLOC(Block.nin*sizeof(void *)))==NULL) {
      FREE(Block.insz);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    /* 4 : model.in2  */
    n    = MlistGetFieldNumber(il1, "in2");
    ilh2 = (int *) (listentry(il1,n));
    mh2  = ilh2[1];
    nh2  = ilh2[2];
    /* check value of in2 */
    for (i=0;i<(mh2*nh2);i++) {
      if ((*((double *)(&ilh2[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Size. in2(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh2[4]) + i)));
        FREE(Block.insz);
        FREE(Block.inptr);
        return 0;
      }
    }
    /* 5 : model.intyp  */
    n    = MlistGetFieldNumber(il1, "intyp");
    ilh3 = (int *) (listentry(il1,n));
    mh3  = ilh3[1];
    nh3  = ilh3[2];
    /* check value of intyp */
    for (i=0;i<(mh3*nh3);i++) {
      if ((*((double *)(&ilh3[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Type. intyp(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh3[4]) + i)));
        FREE(Block.insz);
        FREE(Block.inptr);
        return 0;
      }
    }
    if (((mh*nh)==(mh2*nh2)) && (((mh*nh)==(mh3*nh3)))) {
      for(i=0;i<Block.nin;i++) {
        Block.insz[i]=(int) *((double *)(&ilh[4]) + i);
        Block.insz[Block.nin+i]=(int) *((double *)(&ilh2[4]) + i);
        type=*((double *)(&ilh3[4]) + i);
        if (type==1) Block.insz[2*Block.nin+i]=10;
        else if (type==2) Block.insz[2*Block.nin+i]=11;
        else if (type==3) Block.insz[2*Block.nin+i]=84;
        else if (type==4) Block.insz[2*Block.nin+i]=82;
        else if (type==5) Block.insz[2*Block.nin+i]=81;
        else if (type==6) Block.insz[2*Block.nin+i]=814;
        else if (type==7) Block.insz[2*Block.nin+i]=812;
        else if (type==8) Block.insz[2*Block.nin+i]=811;
        else Block.insz[2*Block.nin+i]=10;
      }
    }
    else {
      for(i=0;i<Block.nin;i++) {
        Block.insz[i]=(int) *((double *)(&ilh[4]) + i);
        Block.insz[Block.nin+i]=1;
        Block.insz[2*Block.nin+i]=10;
      }
    }
    for(i=0;i<Block.nin;i++) {
      switch(Block.insz[2*Block.nin+i])
      {
       case 10  : if ((Block.inptr[i]=(double *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(double)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_d = (double *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_d[j]=0.;
                  }
                  break;
       case 11  : if ((Block.inptr[i]=(double *) \
                        MALLOC(2*Block.insz[i]*Block.insz[Block.nin+i]*sizeof(double)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_d = (double *) Block.inptr[i];
                  for(j=0;j<2*Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_d[j]=0.;
                  }
                  break;
       case 84  : if ((Block.inptr[i]=(long *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(long)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_l = (long *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_l[j]=0;
                  }
                  break;
       case 82  : if ((Block.inptr[i]=(short *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(short)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_s = (short *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_s[j]=0;
                  }
                  break;
       case 81  : if ((Block.inptr[i]=(char *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(char)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_c = (char *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_c[j]=0;
                  }
                  break;
       case 814 : if ((Block.inptr[i]=(unsigned long *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned long)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_ul = (unsigned long *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_ul[j]=0;
                  }
                  break;
       case 812 : if ((Block.inptr[i]=(unsigned short *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned short)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_us = (unsigned short *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_us[j]=0;
                  }
                  break;
       case 811 : if ((Block.inptr[i]=(unsigned char *) \
                        MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned char)))==NULL) {
                    for(j=0;j<i;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_uc = (unsigned char *) Block.inptr[i];
                  for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) {
                    ptr_uc[j]=0;
                  }
                  break;
      }
    }
  }

  /* check output ports */
  /* 6 : model.out  */
  n            = MlistGetFieldNumber(il1, "out");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nout   = mh*nh;
  Block.outsz  = NULL;
  Block.outptr = NULL;
  if (mh*nh!=0) {
    /* check value of out */
    for (i=0;i<Block.nout;i++) {
      if ((*((double *)(&ilh[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Size. out(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh[4]) + i)));
        for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
        FREE(Block.inptr);
        FREE(Block.insz);
        return 0;
      }
    }
    /* alloc outsz */
    if ((Block.outsz=(int *) MALLOC(Block.nout*3*sizeof(int)))==NULL) {
      Scierror(888,"%s : Allocation error.\n",fname);
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      return 0;
    }
    /* alloc outptr */
    if ((Block.outptr=(void **) MALLOC(Block.nout*sizeof(void *)))==NULL) {
      FREE(Block.outsz);
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    /* 7 : model.out2  */
    n    = MlistGetFieldNumber(il1, "out2");
    ilh2 = (int *) (listentry(il1,n));
    mh2  = ilh2[1];
    nh2  = ilh2[2];
    /* check value of out2 */
    for (i=0;i<(mh2*nh2);i++) {
      if ((*((double *)(&ilh2[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Size. out2(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh2[4]) + i)));
        for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
        FREE(Block.insz);
        FREE(Block.inptr);
        FREE(Block.outptr);
        FREE(Block.outsz);
        return 0;
      }
    }
    /* 8 : model.outtyp  */
    n    = MlistGetFieldNumber(il1, "outtyp");
    ilh3 = (int *) (listentry(il1,n));
    mh3  = ilh3[1];
    nh3  = ilh3[2];
    /* check value of intyp */
    for (i=0;i<(mh3*nh3);i++) {
      if ((*((double *)(&ilh3[4]) + i)) <= 0.) {
        Scierror(888,"%s : Undetermined Type. outtyp(%d)=%d. Please adjust your model.\n",\
                 fname,i+1,(int)(*((double *)(&ilh3[4]) + i)));
        FREE(Block.insz);
        FREE(Block.inptr);
        FREE(Block.outptr);
        FREE(Block.outsz);
        return 0;
      }
    }
    if (((mh*nh)==(mh2*nh2)) && (((mh*nh)==(mh3*nh3)))) {
      for(i=0;i<Block.nout;i++) {
        Block.outsz[i]=(int) *((double *)(&ilh[4]) + i);
        Block.outsz[Block.nout+i]=(int) *((double *)(&ilh2[4]) + i);
        type=*((double *)(&ilh3[4]) + i);
        if (type==1) Block.outsz[2*Block.nout+i]=10;
        else if (type==2) Block.outsz[2*Block.nout+i]=11;
        else if (type==3) Block.outsz[2*Block.nout+i]=84;
        else if (type==4) Block.outsz[2*Block.nout+i]=82;
        else if (type==5) Block.outsz[2*Block.nout+i]=81;
        else if (type==6) Block.outsz[2*Block.nout+i]=814;
        else if (type==7) Block.outsz[2*Block.nout+i]=812;
        else if (type==8) Block.outsz[2*Block.nout+i]=811;
        else Block.outsz[2*Block.nout+i]=10;
      }
    }
    else {
      for(i=0;i<Block.nout;i++) {
        Block.outsz[i]=(int) *((double *)(&ilh[4]) + i);
        Block.outsz[Block.nout+i]=1;
        Block.outsz[2*Block.nout+i]=10;
      }
    }
    for(i=0;i<Block.nout;i++) {
      switch(Block.outsz[2*Block.nout+i])
      {
       case 10  : if ((Block.outptr[i]=(double *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(double)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_d = (double *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_d[j]=0.;
                  }
                  break;
       case 11  : if ((Block.outptr[i]=(double *) \
                        MALLOC(2*Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(double)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_d = (double *) Block.outptr[i];
                  for(j=0;j<2*Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_d[j]=0.;
                  }
                  break;
       case 84  : if ((Block.outptr[i]=(long *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(long)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_l = (long *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_l[j]=0;
                  }
                  break;
       case 82  : if ((Block.outptr[i]=(short *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(short)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_s = (short *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_s[j]=0;
                  }
                  break;
       case 81  : if ((Block.outptr[i]=(char *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(char)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_c = (char *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_c[j]=0;
                  }
                  break;
       case 814 : if ((Block.outptr[i]=(unsigned long *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(unsigned long)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_ul = (unsigned long *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_ul[j]=0;
                  }
                  break;
       case 812 : if ((Block.outptr[i]=(unsigned short *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(unsigned short)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_us = (unsigned short *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_us[j]=0;
                  }
                  break;
       case 811 : if ((Block.outptr[i]=(unsigned char *) \
                        MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(unsigned char)))==NULL) {
                    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
                    FREE(Block.inptr);
                    FREE(Block.insz);
                    for(j=0;j<i;j++) FREE(Block.outptr[j]);
                    FREE(Block.outptr);
                    FREE(Block.outsz);
                    Scierror(888,"%s : Allocation error.\n",fname);
                    return 0;
                  }
                  ptr_uc = (unsigned char *) Block.outptr[i];
                  for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) {
                    ptr_uc[j]=0;
                  }
                  break;
      }
    }
  }

  /* event input port */
  /* 9 : model.evtin  */

  /* event output port  */
  /* 10 : model.evtout  */
  n            = MlistGetFieldNumber(il1, "evtout");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nevout = mh*nh;
  Block.evout  = NULL;
  if (mh*nh!=0) {
    if ((Block.evout=(double *) MALLOC(Block.nevout*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    n            = MlistGetFieldNumber(il1, "firing");
    ilh2         = (int *) (listentry(il1,n));
    mh2          = ilh2[1];
    nh2          = ilh2[2];
    if ((mh*nh)==(mh2*nh2)) {
      for(j=0;j<Block.nevout;j++) {
        Block.evout[j]=*((double *)(&ilh2[4]) + j);
      }
    }
    else {
      for(j=0;j<Block.nevout;j++) {
        Block.evout[j]=-1.0;
      }
    }
  }

  /* continuous state  */
  /* 11 : model.state  */
  n            = MlistGetFieldNumber(il1, "state");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nx     = mh*nh;
  Block.x      = NULL;
  Block.xprop  = NULL;
  Block.xd     = NULL;
  Block.res    = NULL;
  if (mh*nh!=0) {
    /* x */
    if ((Block.x=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.nx;j++) {
      Block.x[j]=*((double *)(&ilh[4]) + j);
    }
    /* xd */
    if ((Block.xd=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.nx;j++) {
      Block.xd[j]=0.;
    }
    /* xprop */
    if ((Block.xprop=(int *) MALLOC(Block.nx*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.nx;j++) {
      Block.xprop[j]=1;
    }
    /* res */
    /*if (blktyp>10000) {*/
      if ((Block.res=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) {
        for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
        FREE(Block.inptr);
        FREE(Block.insz);
        for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
        FREE(Block.outptr);
        FREE(Block.outsz);
        FREE(Block.evout);
        FREE(Block.x);
        FREE(Block.xd);
        FREE(Block.xprop);
        Scierror(888,"%s : Allocation error.\n",fname);
        return 0;
      }
      for(j=0;j<Block.nx;j++) {
        Block.res[j]=0.;
      }
    /*}*/
  }

  /* discrete state  */
  /* 12 : model.dstate  */
  n            = MlistGetFieldNumber(il1, "dstate");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nz     = mh*nh;
  Block.z      = NULL;
  if (mh*nh!=0) {
    if ((Block.z=(double *) MALLOC(Block.nz*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.nz;j++) {
      Block.z[j]=*((double *)(&ilh[4]) + j);
    }
  }

  /* discrete object state  */
  /* 13 : model.odstate  */
  n            = MlistGetFieldNumber(il1, "odstate");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.noz    = mh*nh;
  Block.ozsz   = NULL;
  Block.oztyp  = NULL;
  Block.ozptr  = NULL;
  if (mh*nh!=0) {
    if ((Block.ozsz=(int *) MALLOC(2*Block.noz*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    if ((Block.oztyp=(int *) MALLOC(Block.noz*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    if ((Block.ozptr=(void **) MALLOC(Block.noz*sizeof(void *)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    for (i=0;i<mh*nh;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block.ozsz[i] = mh2;
      Block.ozsz[Block.noz+i] = nh2;
      if (ilh2[0]==1) {
        if (ilh2[3]==0) {
          Block.oztyp[i] = 10;
          if ((Block.ozptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_d = (double *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_d[j]=*((double *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==1) {
          Block.oztyp[i] = 11;
          if ((Block.ozptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_d = (double *) Block.ozptr[i];
          for(j=0;j<2*mh2*nh2;j++) {
            ptr_d[j]=*((double *)(&ilh2[4]) + j);
          }
        }
      }
      else if (ilh2[0]==8) {
        if (ilh2[3]==4) {
          Block.oztyp[i] = 84;
          if ((Block.ozptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_l = (long *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_l[j]=*((long *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==2) {
          Block.oztyp[i] = 82;
          if ((Block.ozptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_s = (short *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_s[j]=*((short *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==1) {
          Block.oztyp[i] = 81;
          if ((Block.ozptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_c = (char *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_c[j]=*((char *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==14) {
          Block.oztyp[i] = 814;
          if ((Block.ozptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_ul = (unsigned long *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_ul[j]=*((unsigned long *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==12) {
          Block.oztyp[i] = 812;
          if ((Block.ozptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_us = (unsigned short *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_us[j]=*((unsigned short *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==11) {
          Block.oztyp[i] = 811;
          if ((Block.ozptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<i;j++) FREE(Block.ozptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_uc = (unsigned char *) Block.ozptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_uc[j]=*((unsigned char *)(&ilh2[4]) + j);
          }
        }
      }
    }
  }

  /* real parameters */
  /* 14 : model.rpar  */
  n            = MlistGetFieldNumber(il1, "rpar");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nrpar  = mh*nh;
  Block.rpar   = NULL;
  if (mh*nh!=0) {
    if ((Block.rpar=(double *) MALLOC(Block.nrpar*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
    }
    for(j=0;j<Block.nrpar;j++) {
      Block.rpar[j]=*((double *)(&ilh[4]) + j);
    }
  }

  /* integer parameters */
  /* 15 : model.ipar  */
  n            = MlistGetFieldNumber(il1, "ipar");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nipar  = mh*nh;
  Block.ipar   = NULL;
  if (mh*nh!=0) {
    if ((Block.ipar=(int *) MALLOC(Block.nipar*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
    }
    for(j=0;j<Block.nipar;j++) {
      Block.ipar[j]=(int) *((double *)(&ilh[4]) + j);
    }
  }

  /* object parameters */
  /* 16 : model.opar  */
  n             = MlistGetFieldNumber(il1, "opar");
  ilh           = (int *) (listentry(il1,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block.nopar   = mh*nh;
  Block.oparsz  = NULL;
  Block.opartyp = NULL;
  Block.oparptr = NULL;
  if (mh*nh!=0) {
    if ((Block.oparsz=(int *) MALLOC(2*Block.nopar*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    if ((Block.opartyp=(int *) MALLOC(Block.nopar*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      FREE(Block.oparsz);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    if ((Block.oparptr=(void **) MALLOC(Block.nopar*sizeof(void *)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.xprop);
      FREE(Block.res);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      FREE(Block.oparsz);
      FREE(Block.opartyp);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }

    for (i=0;i<mh*nh;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block.oparsz[i] = mh2;
      Block.oparsz[Block.nopar+i] = nh2;
      if (ilh2[0]==1) {
        if (ilh2[3]==0) {
          Block.opartyp[i] = 10;
          if ((Block.oparptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_d = (double *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_d[j]=*((double *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==1) {
          Block.opartyp[i] = 11;
          if ((Block.oparptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_d = (double *) Block.oparptr[i];
          for(j=0;j<2*mh2*nh2;j++) {
            ptr_d[j]=*((double *)(&ilh2[4]) + j);
          }
        }
      }
      else if (ilh2[0]==8) {
        if (ilh2[3]==4) {
          Block.opartyp[i] = 84;
          if ((Block.oparptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_l = (long *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_l[j]=*((long *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==2) {
          Block.opartyp[i] = 82;
          if ((Block.oparptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_s = (short *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_s[j]=*((short *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==1) {
          Block.opartyp[i] = 81;
          if ((Block.oparptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_c = (char *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_c[j]=*((char *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==14) {
          Block.opartyp[i] = 814;
          if ((Block.oparptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_ul = (unsigned long *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_ul[j]=*((unsigned long *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==12) {
          Block.opartyp[i] = 812;
          if ((Block.oparptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_us = (unsigned short *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_us[j]=*((unsigned short *)(&ilh2[4]) + j);
          }
        }
        else if (ilh2[3]==11) {
          Block.opartyp[i] = 811;
          if ((Block.oparptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
            for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
            FREE(Block.inptr);
            FREE(Block.insz);
            for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
            FREE(Block.outptr);
            FREE(Block.outsz);
            FREE(Block.evout);
            FREE(Block.x);
            FREE(Block.xd);
            FREE(Block.xprop);
            FREE(Block.res);
            FREE(Block.z);
            FREE(Block.ozsz);
            FREE(Block.oztyp);
            for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]); 
            FREE(Block.rpar);
            FREE(Block.ipar);
            FREE(Block.oparsz);
            FREE(Block.opartyp);
            for(j=0;j<i;j++) FREE(Block.oparptr[j]);
            Scierror(888,"%s : Allocation error.\n",fname);
            return 0;
          }
          ptr_uc = (unsigned char *) Block.oparptr[i];
          for(j=0;j<mh2*nh2;j++) {
            ptr_uc[j]=*((unsigned char *)(&ilh2[4]) + j);
          }
        }
      }
    }
  }

  /* labels */
  /* 20 : model.label  */
  n            = MlistGetFieldNumber(il1, "label");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.label  = "";
  if (mh*nh!=0) {
    len_str  = ilh[5]-1;
    if (len_str!=0) {
      if ((Block.label=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) {
        for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
        FREE(Block.inptr);
        FREE(Block.insz);
        for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
        FREE(Block.outptr);
        FREE(Block.outsz);
        FREE(Block.evout);
        FREE(Block.x);
        FREE(Block.xd);
        FREE(Block.xprop);
        FREE(Block.res);
        FREE(Block.z);
        FREE(Block.ozsz);
        FREE(Block.oztyp);
        for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
        FREE(Block.rpar);
        FREE(Block.ipar);
        FREE(Block.oparsz);
        FREE(Block.opartyp);
        for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
        Scierror(888,"%s : Allocation error.\n",fname);
        return 0;
      }
      Block.label[len_str]='\0';
      C2F(cvstr)(&len_str,&ilh[6],Block.label,(j=1,&j),len_str);
    }
  }

  /* zero crossing */
  /* 21 : model.nzcross  */
  n            = MlistGetFieldNumber(il1, "nzcross");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.ng     = (int)*((double *)(&ilh[4]));
  Block.g      = NULL;
  Block.jroot  = NULL;
  if (Block.ng!=0) {
    if ((Block.g=(double *) MALLOC(Block.ng*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.res);
      FREE(Block.xprop);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      FREE(Block.oparsz);
      FREE(Block.opartyp);
      for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
      FREE(Block.label);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.ng;j++) {
      Block.g[j]=0.;
    }
    if ((Block.jroot=(int *) MALLOC(Block.ng*sizeof(int)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.res);
      FREE(Block.xprop);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      FREE(Block.oparsz);
      FREE(Block.opartyp);
      for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
      FREE(Block.label);
      FREE(Block.g);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.ng;j++) {
      Block.jroot[j]=0;
    }
  }

  /* mode */
  /* 22 : model.nmode  */
  n            = MlistGetFieldNumber(il1, "nmode");
  ilh          = (int *) (listentry(il1,n));
  mh           = ilh[1];
  nh           = ilh[2];
  Block.nmode  = (int)*((double *)(&ilh[4]));
  Block.mode  = NULL;
  if (Block.nmode!=0) {
    if ((Block.mode=(int *) MALLOC(Block.nmode*sizeof(double)))==NULL) {
      for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
      FREE(Block.inptr);
      FREE(Block.insz);
      for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
      FREE(Block.outptr);
      FREE(Block.outsz);
      FREE(Block.evout);
      FREE(Block.x);
      FREE(Block.xd);
      FREE(Block.res);
      FREE(Block.xprop);
      FREE(Block.z);
      FREE(Block.ozsz);
      FREE(Block.oztyp);
      for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
      FREE(Block.rpar);
      FREE(Block.ipar);
      FREE(Block.oparsz);
      FREE(Block.opartyp);
      for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
      FREE(Block.label);
      FREE(Block.g);
      FREE(Block.jroot);
      Scierror(888,"%s : Allocation error.\n",fname);
      return 0;
    }
    for(j=0;j<Block.nmode;j++) {
      Block.mode[j]=0;
    }
  }

  /* work */
  if ((Block.work=(void **) MALLOC(sizeof(void *)))==NULL) {
    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
    FREE(Block.inptr);
    FREE(Block.insz);
    for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
    FREE(Block.outptr);
    FREE(Block.outsz);
    FREE(Block.evout);
    FREE(Block.x);
    FREE(Block.xd);
    FREE(Block.res);
    FREE(Block.xprop);
    FREE(Block.z);
    FREE(Block.ozsz);
    FREE(Block.oztyp);
    for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
    FREE(Block.rpar);
    FREE(Block.ipar);
    FREE(Block.oparsz);
    FREE(Block.opartyp);
    for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
    FREE(Block.label);
    FREE(Block.g);
    FREE(Block.jroot);
    FREE(Block.mode);
    Scierror(888,"%s : Allocation error.\n",fname);
    return 0;
  } 
  *Block.work = NULL;

  TopSave = Top;

  ierr=createblklist(&Block, &ierr, -1, Block.type);

  for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
  FREE(Block.inptr);
  FREE(Block.insz);
  for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
  FREE(Block.outptr);
  FREE(Block.outsz);
  FREE(Block.evout);
  FREE(Block.x);
  FREE(Block.xd);
  FREE(Block.res);
  FREE(Block.z);
  FREE(Block.ozsz);
  FREE(Block.oztyp);
  for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
  FREE(Block.rpar);
  FREE(Block.ipar);
  FREE(Block.oparsz);
  FREE(Block.opartyp);
  for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
  if (len_str!=0) FREE(Block.label);
  FREE(Block.g);
  FREE(Block.jroot);
  FREE(Block.mode);
  Top = TopSave;

  CreateVar(2,"t",&nblklst,(j=1,&j),&l_tmp);
  LhsVar(1) = 2;

  return 0;
}
/*--------------------------------------------------------------------------*/
/* intcallblk  : Call a scicos block defined by
 * a scicos_block structure.
 *
 * [Block] =callblk(Block,flag,t)
 *
 */
int sci_callblk(char *fname,unsigned long fname_len)
{
  /* auxilary variables -dimension and address-
   * for scilab stack variables
   */
  int *il1;
  int m1,n1;
  int *il2_1;
  int m2_1,n2_1;
  int *il2;
  int m2,n2;
  int *il3;
  int m3,n3;

  /* local variable */
  int len_str;
  char *str;
  int ierr,ret;
  int j;
  int TopSave;
  int l_tmp;

  /* length of the scilab list scicos struct */
  int nblklst=40;

  /* variable for callf */
  int flag;
  double t;
  scicos_block Block;

  /* check number of rhs/lhs param */
  CheckRhs(3,3);
  CheckLhs(1,1);

  /* check rhs 1 (input scilab structure) */
  il1 = (int *) GetData(1);
  m1  = il1[1];
  n1  = il1[2];
  if (il1[0]!=16) {
    Scierror(888,"%s : First argument must be a scicos_block typed list.\n",fname);
    return 0;
  }
  il2_1 = (int *) (listentry(il1,1));
  m2_1  = il2_1[1];
  n2_1  = il2_1[2];
  if ((il2_1[0]!=10) || ((m2_1*n2_1)!=nblklst)) {
    Scierror(888,"%s : First argument must be a valid scicos_block typed list.\n",fname);
    return 0;
  }
  len_str = il2_1[5]-1;
  if (len_str!=0) {
    if ((str=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) {
      Scierror(888,"%s : Memory allocation error.\n",fname);
      return 0;
    }
    str[len_str]='\0';
    C2F(cvstr)(&len_str,&il2_1[5+nblklst],str,(j=1,&j),len_str);
    ret = strcmp("scicos_block", str);
    FREE(str);
    if (ret!=0) {
      Scierror(888,"%s : First argument must be a valid scicos_block typed list.\n",fname);
      return 0;
    }
  }
  else {
    Scierror(888,"%s : First argument must be a valid scicos_block typed list.\n",fname);
    return 0;
  }

  /* convert scilab scicos struct to a C scicos struct */
  ret=extractblklist(il1, &Block, &ierr);

  /* error table */
  switch (ierr)
  {
    case -39 : Scierror(888,"%s : Memory allocation error.\n",fname);
               break;

    case 98  : Scierror(888,"%s : First argument must be a valid scicos_block typed list.\n",fname);
               break;

    default: break;
  }
  if (ierr!=0) {
    FREE(Block.z);
    FREE(Block.ozsz);
    FREE(Block.oztyp);
    for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
    FREE(Block.ozptr);
    FREE(Block.x);
    FREE(Block.xd);
    FREE(Block.xprop);
    FREE(Block.res);
    FREE(Block.insz);
    for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
    FREE(Block.inptr);
    FREE(Block.outsz);
    for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
    FREE(Block.outptr);
    FREE(Block.evout);
    FREE(Block.rpar);
    FREE(Block.ipar);
    FREE(Block.oparsz);
    FREE(Block.opartyp);
    for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
    FREE(Block.oparptr);
    FREE(Block.g);
    FREE(Block.jroot);
    if (strlen(Block.label)!=0) FREE(Block.label);
    FREE(Block.mode);
    FREE(Block.xprop);
    return 0;
  }

  /* check rhs 2 (flag) */
  il2 = (int *) GetData(2);
  m2  = il2[1];
  n2  = il2[2];
  if ((il2[0]!=1) || (m2*n2!=1)){
    Scierror(888,"%s : Second argument must be scalar.\n",fname);
    return 0;
  }
  flag=(int) *((double *)(&il2[4]));

  /* check rhs 3 (time) */
  il3 = (int *) GetData(3);
  m3  = il3[1];
  n3  = il3[2];
  if ((il3[0]!=1) || (m3*n3!=1)){
    Scierror(888,"%s : Third argument must be scalar.\n",fname);
    return 0;
  }
  t=*((double *)(&il3[4]));

  /* call block */
  callf(&t, &Block, &flag);

  /* build output  scilab structure */
  TopSave = Top;

  ierr=createblklist(&Block, &ierr, -1, Block.type);

  FREE(Block.z);
  FREE(Block.ozsz);
  FREE(Block.oztyp);
  for(j=0;j<Block.noz;j++) FREE(Block.ozptr[j]);
  FREE(Block.ozptr);
  FREE(Block.x);
  FREE(Block.xd);
  FREE(Block.res);
  FREE(Block.insz);
  for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
  FREE(Block.inptr);
  FREE(Block.outsz);
  for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
  FREE(Block.outptr);
  FREE(Block.evout);
  FREE(Block.rpar);
  FREE(Block.ipar);
  FREE(Block.oparsz);
  FREE(Block.opartyp);
  for(j=0;j<Block.nopar;j++) FREE(Block.oparptr[j]);
  FREE(Block.oparptr);
  FREE(Block.g);
  FREE(Block.jroot);
  if (strlen(Block.label)!=0) FREE(Block.label);
  FREE(Block.mode);

  Top = TopSave;

  CreateVar(4,"t",&nblklst,(j=1,&j),&l_tmp);
  LhsVar(1)=4;

  return 0;
}
/*--------------------------------------------------------------------------*/
/* extractblklist : create a scicos_block C structure from
 * a scicos_block scilab structure.
 *
 * Input : il : the ptr of the scicos_block scilab structure
 *              in the stack
 *
 * Output : Block : C scicos_block structure
 *          ierr  : an error number (ifany)
 *
 * initial rev 13/11/07, Alan
 */

int extractblklist(int *il, scicos_block *Block, int *ierr)
{

  /*
   * error table :
   *  ierr = 0    : no errors
   *  ierr = -39 : allocation error
   *  ierr = 98  : invalid field name
   *
   */

  /* variables for scilab stack */
  int *ilh = NULL;
  int mh = 0, nh = 0;
  int *ilh2 = NULL;
  int mh2 = 0, nh2 = 0;
  int len_str = 0;

  /* local counter variables */
  int n = 0;
  int i = 0, j = 0;

  *ierr = 0;

  /* 2 - nevprt */
  n            = MlistGetFieldNumber(il, "nevprt");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nevprt = (int) *((double *)(&ilh[4]));

  /* 3 - funpt */
  n            = MlistGetFieldNumber(il, "funpt");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  i             = (int) *((double *)(&ilh[4]));
  Block->funpt  = (voidg) i;

  /* 4 - type */
  n            = MlistGetFieldNumber(il, "type");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->type   = (int) *((double *)(&ilh[4]));

  /* 5 - scsptr */
  n            = MlistGetFieldNumber(il, "scsptr");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->scsptr = (int) *((double *)(&ilh[4]));

  /* 6 - nz */
  n            = MlistGetFieldNumber(il, "nz");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nz     = (int) *((double *)(&ilh[4]));

  /* 7 - z */
  n            = MlistGetFieldNumber(il, "z");
  if (n==-1) {
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->z      = NULL;
  if (mh*nh!=0) {
    if ((Block->z=(double *) MALLOC(Block->nz*sizeof(double)))==NULL) {
      *ierr = -39;
      return 0;
    }
    for(j=0;j<Block->nz;j++) {
      Block->z[j]=*((double *)(&ilh[4]) + j);
    }
  }

  /* 8 - noz */
  n            = MlistGetFieldNumber(il, "noz");
  if (n==-1) {
    FREE(Block->z);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->noz    = (int) *((double *)(&ilh[4]));

  /* 9 - ozsz */
  n            = MlistGetFieldNumber(il, "ozsz");
  if (n==-1) {
    FREE(Block->z);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->ozsz   = NULL;
  if (mh*nh!=0) {
    if ((Block->ozsz=(int *) MALLOC(2*Block->noz*sizeof(int)))==NULL) {
      FREE(Block->z);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<2*Block->noz;j++) {
    Block->ozsz[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 10 - oztyp */
  n            = MlistGetFieldNumber(il, "oztyp");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->oztyp  = NULL;
  if (mh*nh!=0) {
    if ((Block->oztyp=(int *) MALLOC(Block->noz*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<Block->noz;j++) {
    Block->oztyp[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 11 - oz */
  n            = MlistGetFieldNumber(il, "oz");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->ozptr  = NULL;
  if (Block->noz!=0) {
    if ((Block->ozptr=(void **) MALLOC(Block->noz*sizeof(void *)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      *ierr = -39;
      return 0;
    }
    for(i=0;i<Block->noz;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block->ozptr[i] = NULL;
      switch(Block->oztyp[i])
      {
       case 10  : if ((Block->ozptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->ozptr[i], \
                         (double *)(&ilh2[4]), \
                         mh2*nh2*sizeof(double));
                  break;

       case 11  : if ((Block->ozptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->ozptr[i], \
                         (double *)(&ilh2[4]), \
                         2*mh2*nh2*sizeof(double));
                  break;

       case 84  : if ((Block->ozptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((long *)Block->ozptr[i], \
                         (long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(long));
                  break;

       case 82  : if ((Block->ozptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((short *)Block->ozptr[i], \
                         (short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(short));
                  break;

       case 81  : if ((Block->ozptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((char *)Block->ozptr[i], \
                         (char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(char));
                  break;

       case 814 : if ((Block->ozptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned long *)Block->ozptr[i], \
                         (unsigned long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned long));
                  break;

       case 812 : if ((Block->ozptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned short *)Block->ozptr[i], \
                         (unsigned short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned short));
                  break;

       case 811 : if ((Block->ozptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<i;j++) FREE(Block->ozptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned char *)Block->ozptr[i], \
                         (unsigned char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned char));
                  break;
      }
    }
  }

  /* 12 - nx */
  n            = MlistGetFieldNumber(il, "nx");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nx     = (int) *((double *)(&ilh[4]));

  /* 13 - x */
  n            = MlistGetFieldNumber(il, "x");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->x      = NULL;
  if (mh*nh!=0) {
    if ((Block->x=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->x[j]=*((double *)(&ilh[4]) + j);
  }

  /* 14 - xd */
  n            = MlistGetFieldNumber(il, "xd");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->xd     = NULL;
  if (mh*nh!=0) {
    if ((Block->xd=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->xd[j]=*((double *)(&ilh[4]) + j);
  }

  /* 15 - res */
  n            = MlistGetFieldNumber(il, "res");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->res    = NULL;
  if (mh*nh!=0) {
    if ((Block->res=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->res[j]=*((double *)(&ilh[4]) + j);
  }

  /* 16 - nin */
  n            = MlistGetFieldNumber(il, "nin");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nin    = (int) *((double *)(&ilh[4]));

  /* 17 - insz */
  n            = MlistGetFieldNumber(il, "insz");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->insz   = NULL;
  if (mh*nh!=0) {
    if ((Block->insz=(int *) MALLOC(3*Block->nin*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<3*Block->nin;j++) {
    Block->insz[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 18 - inptr */
  n            = MlistGetFieldNumber(il, "inptr");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->inptr  = NULL;
  if (Block->nin!=0) {
    if ((Block->inptr=(void **) MALLOC(Block->nin*sizeof(void *)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      *ierr = -39;
      return 0;
    }
    for(i=0;i<Block->nin;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block->inptr[i] = NULL;
      switch(Block->insz[2*Block->nin+i])
      {
       case 10  : if ((Block->inptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->inptr[i], \
                         (double *)(&ilh2[4]), \
                         mh2*nh2*sizeof(double));
                  break;

       case 11  : if ((Block->inptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->inptr[i], \
                         (double *)(&ilh2[4]), \
                         2*mh2*nh2*sizeof(double));
                  break;

       case 84  : if ((Block->inptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((long *)Block->inptr[i], \
                         (long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(long));
                  break;

       case 82  : if ((Block->inptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((short *)Block->inptr[i], \
                         (short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(short));
                  break;

       case 81  : if ((Block->inptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((char *)Block->inptr[i], \
                         (char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(char));
                  break;

       case 814 : if ((Block->inptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned long *)Block->inptr[i], \
                         (unsigned long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned long));
                  break;

       case 812 : if ((Block->inptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned short *)Block->inptr[i], \
                         (unsigned short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned short));
                  break;

       case 811 : if ((Block->inptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<i;j++) FREE(Block->inptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned char *)Block->inptr[i], \
                         (unsigned char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned char));
                  break;
      }
    }
  }

  /* 19 - nout */
  n            = MlistGetFieldNumber(il, "nout");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nout   = (int) *((double *)(&ilh[4]));

  /* 20 - outsz */
  n            = MlistGetFieldNumber(il, "outsz");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->outsz   = NULL;
  if (mh*nh!=0) {
    if ((Block->outsz=(int *) MALLOC(3*Block->nout*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<3*Block->nout;j++) {
    Block->outsz[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 21 - outptr */
  n            = MlistGetFieldNumber(il, "outptr");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->outptr = NULL;
  if (Block->nout!=0) {
    if ((Block->outptr=(void **) MALLOC(Block->nout*sizeof(void *)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      *ierr = -39;
      return 0;
    }
    for(i=0;i<Block->nout;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block->outptr[i] = NULL;
      switch(Block->outsz[2*Block->nout+i])
      {
       case 10  : if ((Block->outptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->outptr[i], \
                         (double *)(&ilh2[4]), \
                         mh2*nh2*sizeof(double));
                  break;

       case 11  : if ((Block->outptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->outptr[i], \
                         (double *)(&ilh2[4]), \
                         2*mh2*nh2*sizeof(double));
                  break;

       case 84  : if ((Block->outptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((long *)Block->outptr[i], \
                         (long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(long));
                  break;

       case 82  : if ((Block->outptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((short *)Block->outptr[i], \
                         (short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(short));
                  break;

       case 81  : if ((Block->outptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((char *)Block->outptr[i], \
                         (char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(char));
                  break;

       case 814 : if ((Block->outptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned long *)Block->outptr[i], \
                         (unsigned long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned long));
                  break;

       case 812 : if ((Block->outptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned short *)Block->outptr[i], \
                         (unsigned short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned short));
                  break;

       case 811 : if ((Block->outptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<i;j++) FREE(Block->outptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned char *)Block->outptr[i], \
                         (unsigned char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned char));
                  break;
      }
    }
  }

  /* 22 - nevout */
  n            = MlistGetFieldNumber(il, "nevout");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nevout = (int) *((double *)(&ilh[4]));

  /* 23 - evout */
  n            = MlistGetFieldNumber(il, "evout");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->evout  = NULL;
  if (mh*nh!=0) {
    if ((Block->evout=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->evout[j]=*((double *)(&ilh[4]) + j);
  }

  /* 24 - nrpar */
  n            = MlistGetFieldNumber(il, "nrpar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nrpar  = (int) *((double *)(&ilh[4]));

  /* 25 - rpar */
  n            = MlistGetFieldNumber(il, "rpar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->rpar   = NULL;
  if (mh*nh!=0) {
    if ((Block->rpar=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->rpar[j]=*((double *)(&ilh[4]) + j);
  }

  /* 26 - nipar */
  n            = MlistGetFieldNumber(il, "nipar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nipar  = (int) *((double *)(&ilh[4]));

  /* 27 - ipar */
  n            = MlistGetFieldNumber(il, "ipar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->ipar   = NULL;
  if (mh*nh!=0) {
    if ((Block->ipar=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->ipar[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 28 - nopar */
  n            = MlistGetFieldNumber(il, "nopar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nopar  = (int) *((double *)(&ilh[4]));

  /* 29 - oparsz */
  n            = MlistGetFieldNumber(il, "oparsz");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->oparsz = NULL;
  if (mh*nh!=0) {
    if ((Block->oparsz=(int *) MALLOC(2*Block->nopar*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<2*Block->nopar;j++) {
    Block->oparsz[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 30 - opartyp */
  n            = MlistGetFieldNumber(il, "opartyp");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->opartyp = NULL;
  if (mh*nh!=0) {
    if ((Block->opartyp=(int *) MALLOC(Block->nopar*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<Block->nopar;j++) {
    Block->opartyp[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 31 - opar */
  n            = MlistGetFieldNumber(il, "opar");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    *ierr = 98;
    return 0;
  }
  ilh            = (int *) (listentry(il,n));
  mh             = ilh[1];
  nh             = ilh[2];
  Block->oparptr = NULL;
  if (Block->nopar!=0) {
    if ((Block->oparptr=(void **) MALLOC(Block->nopar*sizeof(void *)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      FREE(Block->opartyp);
      *ierr = -39;
      return 0;
    }
    for(i=0;i<Block->nopar;i++) {
      ilh2 = (int *) (listentry(ilh,i+1));
      mh2  = ilh2[1];
      nh2  = ilh2[2];
      Block->oparptr[i] = NULL;
      switch(Block->opartyp[i])
      {
       case 10  : if ((Block->oparptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->oparptr[i], \
                         (double *)(&ilh2[4]), \
                         mh2*nh2*sizeof(double));
                  break;

       case 11  : if ((Block->oparptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((double *)Block->oparptr[i], \
                         (double *)(&ilh2[4]), \
                         2*mh2*nh2*sizeof(double));
                  break;

       case 84  : if ((Block->oparptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((long *)Block->oparptr[i], \
                         (long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(long));
                  break;

       case 82  : if ((Block->oparptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((short *)Block->oparptr[i], \
                         (short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(short));
                  break;

       case 81  : if ((Block->oparptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((char *)Block->oparptr[i], \
                         (char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(char));
                  break;

       case 814 : if ((Block->oparptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned long *)Block->oparptr[i], \
                         (unsigned long *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned long));
                  break;

       case 812 : if ((Block->oparptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned short *)Block->oparptr[i], \
                         (unsigned short *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned short));
                  break;

       case 811 : if ((Block->oparptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) {
                    FREE(Block->z);
                    FREE(Block->ozsz);
                    FREE(Block->oztyp);
                    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
                    FREE(Block->ozptr);
                    FREE(Block->x);
                    FREE(Block->xd);
                    FREE(Block->res);
                    FREE(Block->insz);
                    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
                    FREE(Block->inptr);
                    FREE(Block->outsz);
                    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
                    FREE(Block->outptr);
                    FREE(Block->evout);
                    FREE(Block->rpar);
                    FREE(Block->ipar);
                    FREE(Block->oparsz);
                    FREE(Block->opartyp);
                    for(j=0;j<i;j++) FREE(Block->oparptr[j]);
                    *ierr = -39;
                    return 0;
                  }
                  memcpy((unsigned char *)Block->oparptr[i], \
                         (unsigned char *)(&ilh2[4]), \
                         mh2*nh2*sizeof(unsigned char));
                  break;
      }
    }
  }

  /* 32 - ng */
  n            = MlistGetFieldNumber(il, "ng");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->ng     = (int) *((double *)(&ilh[4]));

  /* 33 - g */
  n            = MlistGetFieldNumber(il, "g");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->g      = NULL;
  if (mh*nh!=0) {
    if ((Block->g=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      FREE(Block->opartyp);
      for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
      FREE(Block->oparptr);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->g[j]=*((double *)(&ilh[4]) + j);
  }

  /* 34 - ztyp */
  n            = MlistGetFieldNumber(il, "ztyp");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->ztyp   = (int) *((double *)(&ilh[4]));

  /* 35 - jroot */
  n            = MlistGetFieldNumber(il, "jroot");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->jroot  = NULL;
  if (mh*nh!=0) {
    if ((Block->jroot=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      FREE(Block->opartyp);
      for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
      FREE(Block->oparptr);
      FREE(Block->g);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->jroot[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 36 - label */
  n            = MlistGetFieldNumber(il, "label");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    FREE(Block->jroot);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->label  = "";
  if (mh*nh!=0) {
    len_str  = ilh[5]-1;
    if (len_str!=0) {
      if ((Block->label=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) {
        FREE(Block->z);
        FREE(Block->ozsz);
        FREE(Block->oztyp);
        for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
        FREE(Block->ozptr);
        FREE(Block->x);
        FREE(Block->xd);
        FREE(Block->res);
        FREE(Block->insz);
        for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
        FREE(Block->inptr);
        FREE(Block->outsz);
        for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
        FREE(Block->outptr);
        FREE(Block->evout);
        FREE(Block->rpar);
        FREE(Block->ipar);
        FREE(Block->oparsz);
        FREE(Block->opartyp);
        for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
        FREE(Block->oparptr);
        FREE(Block->g);
        FREE(Block->jroot);
        *ierr = -39;
        return 0;
      }
      Block->label[len_str]='\0';
      C2F(cvstr)(&len_str,&ilh[6],Block->label,(j=1,&j),len_str);
    }
  }

  /* 37 - work*/
  n            = MlistGetFieldNumber(il, "work");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    FREE(Block->jroot);
    if (len_str!=0) FREE(Block->label);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->work = (void**) ((int) *((double *)(&ilh[4])));

  /* 38 - nmode*/
  n            = MlistGetFieldNumber(il, "nmode");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    FREE(Block->jroot);
    if (len_str!=0) FREE(Block->label);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->nmode  = (int) *((double *)(&ilh[4]));

  /* 39 - mode */
  n            = MlistGetFieldNumber(il, "mode");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    FREE(Block->jroot);
    if (len_str!=0) FREE(Block->label);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->mode   = NULL;
  if (mh*nh!=0) {
    if ((Block->mode=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      FREE(Block->opartyp);
      for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
      FREE(Block->oparptr);
      FREE(Block->g);
      FREE(Block->jroot);
      if (len_str!=0) FREE(Block->label);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->mode[j]=(int) *((double *)(&ilh[4]) + j);
  }

  /* 40 - xprop */
  n            = MlistGetFieldNumber(il, "xprop");
  if (n==-1) {
    FREE(Block->z);
    FREE(Block->ozsz);
    FREE(Block->oztyp);
    for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
    FREE(Block->ozptr);
    FREE(Block->x);
    FREE(Block->xd);
    FREE(Block->res);
    FREE(Block->insz);
    for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
    FREE(Block->inptr);
    FREE(Block->outsz);
    for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
    FREE(Block->outptr);
    FREE(Block->evout);
    FREE(Block->rpar);
    FREE(Block->ipar);
    FREE(Block->oparsz);
    FREE(Block->opartyp);
    for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
    FREE(Block->oparptr);
    FREE(Block->g);
    FREE(Block->jroot);
    if (len_str!=0) FREE(Block->label);
    FREE(Block->mode);
    *ierr = 98;
    return 0;
  }
  ilh           = (int *) (listentry(il,n));
  mh            = ilh[1];
  nh            = ilh[2];
  Block->xprop  = NULL;
  if (mh*nh!=0) {
    if ((Block->xprop=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) {
      FREE(Block->z);
      FREE(Block->ozsz);
      FREE(Block->oztyp);
      for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
      FREE(Block->ozptr);
      FREE(Block->x);
      FREE(Block->xd);
      FREE(Block->res);
      FREE(Block->insz);
      for(j=0;j<Block->nin;j++) FREE(Block->inptr[j]);
      FREE(Block->inptr);
      FREE(Block->outsz);
      for(j=0;j<Block->nout;j++) FREE(Block->outptr[j]);
      FREE(Block->outptr);
      FREE(Block->evout);
      FREE(Block->rpar);
      FREE(Block->ipar);
      FREE(Block->oparsz);
      FREE(Block->opartyp);
      for(j=0;j<Block->nopar;j++) FREE(Block->oparptr[j]);
      FREE(Block->oparptr);
      FREE(Block->g);
      FREE(Block->jroot);
      if (len_str!=0) FREE(Block->label);
      FREE(Block->mode);
      *ierr = -39;
      return 0;
    }
  }
  for(j=0;j<(mh*nh);j++) {
    Block->xprop[j]=(int) *((double *)(&ilh[4]) + j);
  }

 return 0;
}
/*--------------------------------------------------------------------------*/
