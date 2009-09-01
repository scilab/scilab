/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "scicos_block4.h"
#include "Scierror.h"
#include "scicos.h"
#include "sciblk4.h"
#include "import.h"
#include "createblklist.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* Define external function */
extern int C2F(scierr)();
extern void C2F(scifunc)();
extern int *listentry(int *header, int i);
extern void C2F(itosci)();
/*--------------------------------------------------------------------------*/
static int sci2var(void *x,void *y,int typ_var);
/*--------------------------------------------------------------------------*/
void sciblk4(scicos_block *Blocks, int flag)
{
  /*counter and address variable declaration*/
  int i = 0,j = 0,k = 0,topsave = 0;
  int ierr = 0;
  int kfun = 0;

  int *header = NULL,ne1 = 0;
  double *le111 = NULL;

  int *il_xd = NULL, *il_res = NULL, *il_out = NULL, *il_outptr = NULL;
  int *il_xprop = NULL;

  int *il_z = NULL, *il_oz = NULL, *il_ozptr = NULL, *il_x = NULL;
  int *il_mode = NULL, *il_evout = NULL, *il_g = NULL;
  double *l_mode = NULL;
  double *l_xprop = NULL;

  /* variable for output typed port */
  int nout = 0;

  int nv = 0,mv = 0;
  int *ptr = NULL, *funtyp = NULL;

  /* set number of left and right hand side parameters */
  int mlhs = 1,mrhs = 2;

  /* Save Top counter */
  topsave = Top;

  /* Retrieve block number */
  kfun = get_block_number();

  /* Retrieve funtyp by import structure */
  strcpy(C2F(cha1).buf,"funtyp");
  ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
  if (ierr==0) goto err;
  funtyp = (int *) ptr;

  /****************************
   * create scilab tlist Blocks
   ****************************/
  if((createblklist(&Blocks[0], &ierr,(i=-1),funtyp[kfun-1]))==0) goto err;

  /* * flag * */
  C2F(itosci)(&flag,(i=1,&i),(j=1,&j));
  if (C2F(scierr)()!=0) goto err;

  /**********************
   * Call scilab function
   **********************/
  C2F(scifunc)(&mlhs,&mrhs);
  if (C2F(scierr)()!=0) goto err;

  /***************************
   * Update C block structure
   **************************/
  /* get header of output variable Blocks of sciblk4 */
  header = (int *) stk(*Lstk(Top));

  /* switch to appropriate flag */
  switch (flag)
  {

  /**************************
   * update continuous state
   **************************/
   case 0 :
   {
    if (Blocks[0].nx != 0)
    {
     /* 14 - xd */
     il_xd = (int *) listentry(header,14);
     ierr=sci2var(il_xd,Blocks[0].xd,SCSREAL_N); /* double */
     if (ierr!=0) goto err;

     if ((funtyp[kfun-1]==10004) || (funtyp[kfun-1]==10005))
     {
      /* 15 - res */
      il_res = (int *) listentry(header,15);
      ierr=sci2var(il_res,Blocks[0].res,SCSREAL_N); /* double */
      if (ierr!=0) goto err;
     }
    }
   }
   break;

  /**********************
   * update output state
   **********************/
   case 1 :
   {
    /* 21 - outptr */
    if (Blocks[0].nout!=0) 
    {
     il_out = (int*) listentry(header,21);
     nout = il_out[1];

     for (j=0;j<nout;j++)
     {
      il_outptr = (int *) listentry(il_out,j+1);
      ierr=sci2var(il_outptr,Blocks[0].outptr[j],Blocks[0].outsz[2*nout+j]);
      if (ierr!=0) goto err;
     }
    }
   }
   break;

  /***********************
   * update discrete state
   ***********************/
   case 2 :
   {
    /* 7 - z */
    if (Blocks[0].nz != 0)
    {
     il_z = (int *) listentry(header,7);
     if(Blocks[0].scsptr>0)
     {
      le111=(double *) listentry(header,7);
      ne1=header[7+2]-header[7+1];
      C2F(unsfdcopy)(&ne1,le111,(i=-1,&i),Blocks[0].z,(j=-1,&j));
     }
     else
     {
      ierr=sci2var(il_z,Blocks[0].z,SCSREAL_N); /* double */
      if (ierr!=0) goto err;
     }
    }

    /* 11 - oz */
    if (Blocks[0].noz != 0)
    {
     il_oz = (int *) listentry(header,11);
     /* C blocks : extract */
     if ((funtyp[kfun-1]==4) || (funtyp[kfun-1]==10004))
     {
      for(j=0;j<Blocks[0].noz;j++)
      {
       il_ozptr = (int *) listentry(il_oz,j+1);
       if (Blocks[0].oztyp[j]==SCSUNKNOW_N)
       { 
        ne1=Blocks[0].ozsz[j];
        C2F(unsfdcopy)(&ne1,(double *)il_ozptr,\
                       (i=1,&i),(double *)Blocks[0].ozptr[j],(k=1,&k));
       }
       else
       {
        ierr=sci2var(il_ozptr,Blocks[0].ozptr[j],Blocks[0].oztyp[j]);
        if (ierr!=0) goto err;
       }
      }
     }
     /* sci blocks : don't extract */
     else if((funtyp[kfun-1]==5) || (funtyp[kfun-1]==10005))
     {
      ne1=Blocks[0].ozsz[0];
      C2F(unsfdcopy)(&ne1,(double *)il_oz,\
                     (i=1,&i),(double *)Blocks[0].ozptr[0],(j=1,&j));
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 13 - x */
     il_x = (int *) listentry(header,13);
     ierr=sci2var(il_x,Blocks[0].x,SCSREAL_N); /* double */
     if (ierr!=0) goto err;

     /* 14 - xd */
     il_xd = (int *) listentry(header,14);
     ierr=sci2var(il_xd,Blocks[0].xd,SCSREAL_N); /* double */
     if (ierr!=0) goto err;
    }
   }
   break;

  /***************************
   * update event output state
   ***************************/
   case 3 :
   {
    /* 23 - evout */
    il_evout = (int *) listentry(header,23);
    ierr=sci2var(il_evout,Blocks[0].evout,SCSREAL_N); /* double */
    if (ierr!=0) goto err;
   }
   break;

  /**********************
   * state initialisation
   **********************/
   case 4 :
   {
    /* 7 - z */
    if (Blocks[0].nz != 0)
    {
     il_z = (int *) listentry(header,7);
     if(Blocks[0].scsptr>0)
     {
      le111=(double *) listentry(header,7);
      ne1=header[7+2]-header[7+1];
      C2F(unsfdcopy)(&ne1,le111,(i=-1,&i),Blocks[0].z,(j=-1,&j));
     }
     else
     {
      ierr=sci2var(il_z,Blocks[0].z,SCSREAL_N); /* double */
      if (ierr!=0) goto err;
     }
    }

    /* 11 - oz */
    if (Blocks[0].noz != 0)
    {
     il_oz = (int *) listentry(header,11);
     /* C blocks : extract */
     if ((funtyp[kfun-1]==4) || (funtyp[kfun-1]==10004))
     {
      for(j=0;j<Blocks[0].noz;j++)
      {
       il_ozptr = (int *) listentry(il_oz,j+1);
       if (Blocks[0].oztyp[j]==SCSUNKNOW_N)
       {
        ne1=Blocks[0].ozsz[j];
        C2F(unsfdcopy)(&ne1,(double *)il_ozptr,\
                       (i=1,&i),(double *)Blocks[0].ozptr[j],(k=1,&k));
       }
       else
       {
        ierr=sci2var(il_ozptr,Blocks[0].ozptr[j],Blocks[0].oztyp[j]);
        if (ierr!=0) goto err;
       }
      }
     }
     /* sci blocks : don't extract */
     else if((funtyp[kfun-1]==5) || (funtyp[kfun-1]==10005))
     {
      ne1=Blocks[0].ozsz[0];
      C2F(unsfdcopy)(&ne1,(double *)il_oz,\
                     (i=1,&i),(double *)Blocks[0].ozptr[0],(j=1,&j));
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 13 - x */
     il_x = (int *) listentry(header,13);
     ierr=sci2var(il_x,Blocks[0].x,SCSREAL_N); /* double */
     if (ierr!=0) goto err;

     /* 14 - xd */
     il_xd = (int *) listentry(header,14);
     ierr=sci2var(il_xd,Blocks[0].xd,SCSREAL_N); /* double */
     if (ierr!=0) goto err;
    }
   }
   break;

  /*********
   * finish
   *********/
   case 5 :
   {
    /* 7 - z */
    if (Blocks[0].nz != 0)
    {
     il_z = (int *) listentry(header,7);
     if(Blocks[0].scsptr>0)
     {
      le111=(double *) listentry(header,7);
      ne1=header[7+2]-header[7+1];
      C2F(unsfdcopy)(&ne1,le111,(i=-1,&i),Blocks[0].z,(j=-1,&j));
     }
     else
     {
      ierr=sci2var(il_z,Blocks[0].z,SCSREAL_N); /* double */
      if (ierr!=0) goto err;
     }
    }

    /* 11 - oz */
    if (Blocks[0].noz != 0)
    {
     il_oz = (int *) listentry(header,11);
     /* C blocks : extract */
     if ((funtyp[kfun-1]==4) || (funtyp[kfun-1]==10004))
     {
      for(j=0;j<Blocks[0].noz;j++)
      {
       il_ozptr = (int *) listentry(il_oz,j+1);
       if (Blocks[0].oztyp[j]==SCSUNKNOW_N)
       {
        ne1=Blocks[0].ozsz[j];
        C2F(unsfdcopy)(&ne1,(double *)il_ozptr,\
                       (i=1,&i),(double *)Blocks[0].ozptr[j],(k=1,&k));
       }
       else
       {
        ierr=sci2var(il_ozptr,Blocks[0].ozptr[j],Blocks[0].oztyp[j]);
        if (ierr!=0) goto err;
       }
      }
     }
     /* sci blocks : don't extract */
     else if((funtyp[kfun-1]==5) || (funtyp[kfun-1]==10005))
     {
      ne1=Blocks[0].ozsz[0];
      C2F(unsfdcopy)(&ne1,(double *)il_oz,\
                     (i=1,&i),(double *)Blocks[0].ozptr[0],(j=1,&j));
     }
    }
   }
   break;

  /*****************************
   * output state initialisation
   *****************************/
   case 6 :
   {
    /* 7 - z */
    if (Blocks[0].nz != 0)
    {
     il_z = (int *) listentry(header,7);
     if(Blocks[0].scsptr>0)
     {
      le111=(double *) listentry(header,7);
      ne1=header[7+2]-header[7+1];
      C2F(unsfdcopy)(&ne1,le111,(i=-1,&i),Blocks[0].z,(j=-1,&j));
     }
     else
     {
      ierr=sci2var(il_z,Blocks[0].z,SCSREAL_N); /* double */
      if (ierr!=0) goto err;
     }
    }

    /* 11 - oz */
    if (Blocks[0].noz != 0)
    {
     il_oz = (int *) listentry(header,11);
     /* C blocks : extract */
     if ((funtyp[kfun-1]==4) || (funtyp[kfun-1]==10004))
     {
      for(j=0;j<Blocks[0].noz;j++)
      {
       il_ozptr = (int *) listentry(il_oz,j+1);
       if (Blocks[0].oztyp[j]==SCSUNKNOW_N)
       {
        ne1=Blocks[0].ozsz[j];
        C2F(unsfdcopy)(&ne1,(double *)il_ozptr,\
                       (i=1,&i),(double *)Blocks[0].ozptr[j],(k=1,&k));
       }
       else
       {
        ierr=sci2var(il_ozptr,Blocks[0].ozptr[j],Blocks[0].oztyp[j]);
        if (ierr!=0) goto err;
       }
      }
     }
     /* sci blocks : don't extract */
     else if((funtyp[kfun-1]==5) || (funtyp[kfun-1]==10005))
     {
      ne1=Blocks[0].ozsz[0];
      C2F(unsfdcopy)(&ne1,(double *)il_oz,\
                     (i=1,&i),(double *)Blocks[0].ozptr[0],(j=1,&j));
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 13 - x */
     il_x = (int *) listentry(header,13);
     ierr=sci2var(il_x,Blocks[0].x,SCSREAL_N); /* double */
     if (ierr!=0) goto err;

     /* 14 - xd */
     il_xd = (int *) listentry(header,14);
     ierr=sci2var(il_xd,Blocks[0].xd,SCSREAL_N); /* double */
     if (ierr!=0) goto err;
    }

    /* 21 - outptr */
    if (Blocks[0].nout!=0) 
    {
     il_out = (int *) listentry(header,21);
     nout = il_out[1];
     for (j=0;j<nout;j++)
     {
      il_outptr = (int *) listentry(il_out,j+1);
      ierr=sci2var(il_outptr,Blocks[0].outptr[j],Blocks[0].outsz[2*nout+j]);
      if (ierr!=0) goto err;
     }
    }
   }
   break;

  /*******************************************
   * define property of continuous time states
   * (algebraic or differential states)
   *******************************************/
   case 7 :
   {
	   if (Blocks[0].nx != 0) {
		   /* 40 - x */
		   il_xprop = (int *) listentry(header,40);
		   l_xprop = (double *)(il_xprop + 4);
		   for (nv=0;nv<Blocks[0].nx;nv++) {
			   Blocks[0].xprop[nv]=(int) l_xprop[nv];
		   }
	   }
   }
   break;

  /****************************
   * zero crossing computation
   ****************************/
   case 9 :
   {
    /* 33 - g */
    il_g = (int *) listentry(header,33);
    ierr=sci2var(il_g,Blocks[0].g,SCSREAL_N); /* double */
    if (ierr!=0) goto err;

     if (get_phase_simulation()==1)
     {
      /* 39 - mode */
      il_mode = (int *) listentry(header,39);
      // Alan, 16/10/07 : fix : mode is an int array
      l_mode = (double *)(il_mode + 4);
      for (nv=0;nv<(il_mode[1]*il_mode[2]);nv++) {
         Blocks[0].mode[nv]=(int) l_mode[nv];
      }
      //ierr=sci2var(il_mode,Blocks[0].mode,SCSINT_N); /* int */
      //if (ierr!=0) goto err;
     }
   }
   break;

  /**********************
   * Jacobian computation
   **********************/
   case 10 :
   {
    if ((funtyp[kfun-1]==10004) || (funtyp[kfun-1]==10005))
    {
     /* 15 - res */
     il_res = (int *) listentry(header,15);
     ierr=sci2var(il_res,Blocks[0].res,SCSREAL_N); /* double */
     if (ierr!=0) goto err;
    }
   }
   break;
  }

  /* Restore initial position Top */
  Top=topsave;
  return;

 /* if error then restore initial position Top
  * and set_block_error with flag -1 */
 err:
  Top=topsave;
  if (ierr!=0) /*var2sci or sci2var error*/
  {
   /* Please update me !*/
   if (ierr < 1000) /*var2sci error*/
   {
    switch (ierr)
    {
     case 1  : Scierror(888,_("%s: error %d. Stack is full.\n"),"var2sci",ierr);
               break;

     case 2  : Scierror(888,_("%s: error %d. No more space on the stack for new data.\n"),"var2sci",ierr);
               break;

     default : Scierror(888,_("%s: error %d. Undefined error.\n"),"var2sci",ierr);
               break;
    }
   }
   else /*sci2var error*/
   {
    switch (ierr)
    {
     case 1001  : Scierror(888,_("%s: error %d. Only int or double object are accepted.\n"),"sci2var",ierr);
                  break;

     case 1002  : Scierror(888,_("%s: error %d. Bad double object sub_type.\n"),"sci2var",ierr);
                  break;

     case 1003  : Scierror(888,_("%s: error %d. Bad int object sub_type.\n"),"sci2var",ierr);
                  break;

     case 1004  : Scierror(888,_("%s: error %d. A type of a scilab object has changed.\n"),"sci2var",ierr);
                  break;

     default    : Scierror(888,_("%s: error %d. Undefined error.\n"),"sci2var",ierr);
                  break;
    }
   }
  }
  set_block_error(-1);
}
/*--------------------------------------------------------------------------*/ 
/* sci2var function to convert scilab object
 * to an array of scicos blocks.
 *
 * Input parameters :
 * *x      : void ptr of scilab object.
 * *y      : void ptr of scicos blocks array.
 * typ_var : integer, type of scicos data :
 *           SCSREAL_N    : double real
 *           SCSCOMPLEX_N : double complex
 *           SCSINT_N     : int
 *           SCSINT8_N    : int8
 *           SCSINT16_N   : int16
 *           SCSINT32_N   : int32
 *           SCSUINT _N   : uint
 *           SCSUINT8_N   : uint8
 *           SCSUINT16_N  : uint16
 *           SCSUINT32_N  : uint32
 *           SCSUNKNOW_N  :
 *
 * Output parameters : int (>1000), error flag 
 *                     (0 if no error)
 *
 * 15/06/06, Alan   : Initial version.
 *
 */

/* prototype */
static int sci2var(void *x,void *y, int typ_var)
{
 /************************************
  * variables and constants définition
  ************************************/
 /* counter and address variable declaration */
 int err,i,j;

 /* variables to store n,m and type of scilab object */
 int n,m,nm,typ,sub_typ;

 /* define int *header */
 int *header;

  /*define all type of accepted ptr */
  SCSREAL_COP *ptr_d, *y_d;
  SCSINT8_COP *ptr_c, *y_c;
  SCSUINT8_COP *ptr_uc, *y_uc;
  SCSINT16_COP *ptr_s, *y_s;
  SCSUINT16_COP *ptr_us, *y_us;
  SCSINT_COP *ptr_i, *y_i;
  SCSUINT_COP *ptr_ui,*y_ui;
  SCSINT32_COP *ptr_l,*y_l;
  SCSUINT32_COP *ptr_ul,*y_ul;

 /*****************************************
  * get header,n,m and typ of scilab object
  *****************************************/
 /* retrieve header address */
 header = (int *) x;

 /* retieve number of rows and columns */
 n = header[1];
 m = header[2];

 /* retrieve type of scilab object */
 typ = header[0];

 /* Test if scilab object is a typed scicos
  * accepted data */
 if ((typ!=1)&(typ!=8))
 {
  err = 1001;
  return err;
 }

 /* retrieve sub_type of scilab object */
 sub_typ =  header[3];

 /* cross type of data checking 
  * and copy scilab object in
  * scicos blocks array structure
  */
 switch (typ)
 {
  case 1 :
          {/*check type of double matrix*/
           if ((sub_typ!=0)&(sub_typ!=1))
           {
            err = 1002;
            return err;
           }
           if ((sub_typ==0)&(typ_var!=SCSREAL_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==1)&(typ_var!=SCSCOMPLEX_N))
           {
            err = 1004;
            return err;
           }

           /*copy double matrix*/
           switch (sub_typ)
           {
            case 0 : nm=n*m;
                     ptr_d = (SCSREAL_COP *) (header+4);
                     y_d = (SCSREAL_COP *) y;
                     C2F(unsfdcopy)(&nm,ptr_d,(i=-1,&i),y_d,(j=-1,&j));
                     break;

            case 1 : nm=2*n*m;
                     ptr_d = (SCSCOMPLEX_COP *) (header+4);
                     y_d = (SCSCOMPLEX_COP *) y;
                     C2F(unsfdcopy)(&nm,ptr_d,(i=-1,&i),y_d,(j=-1,&j));
                     break;
           }
          }
          break;

  case 8 :
          {/*check type of int matrix*/
           if ((sub_typ!=1)  & (sub_typ!=2) &(sub_typ!=4) /
               (sub_typ!=11) & (sub_typ!=12)&(sub_typ!=14))
           {
            err = 1003;
            return err;
           }
           if ((sub_typ==1)&(typ_var!=SCSINT8_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==2)&(typ_var!=SCSINT16_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==4)&(typ_var!=SCSINT_N)&(typ_var!=SCSINT32_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==11)&(typ_var!=SCSUINT8_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==12)&(typ_var!=SCSUINT16_N))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==14)&(typ_var!=SCSUINT_N)&(typ_var!=SCSUINT32_N))
           {
            err = 1004;
            return err;
           }

           /*copy int matrix*/
           switch (typ_var)
           {
            case SCSINT_N    : ptr_i = (SCSINT_COP *) (header+4);
                               y_i = (SCSINT_COP *) y;
                               for(i=0;i<n*m;i++) y_i[i] = ptr_i[i];
                               break;

            case SCSINT8_N   : ptr_c = (SCSINT8_COP *) (header+4);
                               y_c = (SCSINT8_COP *) y;
                               for(i=0;i<n*m;i++) y_c[i] = ptr_c[i];
                               break;

            case SCSINT16_N  : ptr_s = (SCSINT16_COP *) (header+4);
                               y_s = (SCSINT16_COP *) y;
                               for(i=0;i<n*m;i++) y_s[i] = ptr_s[i];
                               break;

            case SCSINT32_N  : ptr_l = (SCSINT32_COP *) (header+4);
                               y_l = (SCSINT32_COP *) y;
                               for(i=0;i<n*m;i++) y_l[i] = ptr_l[i];
                               break;

            case SCSUINT_N   : ptr_ui = (SCSUINT_COP *) (header+4);
                               y_ui = (SCSUINT_COP *) y;
                               for(i=0;i<n*m;i++) y_ui[i] = ptr_ui[i];
                               break;

            case SCSUINT8_N  : ptr_uc = (SCSUINT8_COP *) (header+4);
                               y_uc = (SCSUINT8_COP *) y;
                               for(i=0;i<n*m;i++) y_uc[i] = ptr_uc[i];
                               break;

            case SCSUINT16_N : ptr_us = (SCSUINT16_COP *) (header+4);
                               y_us = (SCSUINT16_COP *) y;
                               for(i=0;i<n*m;i++) y_us[i] = ptr_us[i];
                               break;

            case SCSUINT32_N : ptr_ul = (SCSUINT32_COP *) (header+4);
                               y_ul = (SCSUINT32_COP *) y;
                               for(i=0;i<n*m;i++) y_ul[i] = ptr_ul[i];
                               break;
           }
          }
          break;
 }

 /* return error flag = 0 */
 err = 0;
 return 0;
}
