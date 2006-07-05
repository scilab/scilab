/* Copyright INRIA */
#include <string.h>
#include <stdio.h>

#include "../machine.h"
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#include "../stack-c.h"
#include "scicos_block4.h"

/* Define external function */
extern integer C2F(scierr)();
extern void C2F(scifunc)();
extern int *listentry(int *header, int i);

/* Define var2sci,sci2var,createblklist function */
extern int var2sci(void *x,int n,int m,int typ_var); /*it's now in intcscicos.c*/
extern int createblklist(scicos_block *Blocks, int *ierr, int flag_imp); /*it's now in intcscicos.c*/
int sci2var(void *x,void *y,int typ_var);

/* sciblk4. Run scilab block type 5.
 *
 * Input parameters :
 * Blocks : Tlist
 *    - 1 : Blocks(1)      : !scicos_block nevprt funpt type   scsptr  nz
 *                            z            nx     x     xd     res     nin
 *                            insz         inptr  nout  outsz  outptr  nevout
 *                            evout        nrpar  rpar  nipar  ipar    ng
 *                            g            ztyp   jroot label  work    nmode
 *                            mode !
 *    - 2  : Blocks.nevprt :
 *    - 3  : Blocks.funpt  :
 *    - 4  : Blocks.type   :
 *    - 5  : Blocks.scsptr :
 *    - 6  : Blocks.nz     :
 *    - 7  : Blocks.z      :
 *    - 8  : Blocks.nx     :
 *    - 9  : Blocks.x      :
 *    - 10 : Blocks.xd     :
 *    - 11 : Blocks.res    :
 *    - 12 : Blocks.nin    :
 *    - 13 : Blocks.insz   :
 *    - 14 : Blocks.inptr  :
 *    - 15 : Blocks.nout   :
 *    - 16 : Blocks.outsz  :
 *    - 17 : Blocks.outptr :
 *    - 18 : Blocks.nevout :
 *    - 19 : Blocks.evout  :
 *    - 20 : Blocks.nrpar  :
 *    - 21 : Blocks.rpar   :
 *    - 22 : Blocks.nipar  :
 *    - 23 : Blocks.ipar   :
 *    - 24 : Blocks.ng     :
 *    - 25 : Blocks.g      :
 *    - 26 : Blocks.ztyp   :
 *    - 27 : Blocks.jroot  :
 *    - 28 : Blocks.label  :
 *    - 29 : Blocks.work   :
 *    - 30 : Blocks.nmode  :
 *    - 31 : Blocks.mode   :
 *
 * flag : integer
 *         0 : update continuous state
 *         1 : update output state
 *         2 : update state
 *         3 : update event output state
 *         4 : state initialisation
 *         5 : finish
 *         6 : output state initialisation
 *         7 :
 *         9 :
 *
 * Output parameters :
 * Blocks : Scilab Tlist (updated blocks Scilab list)
 *
 * 08/06/06, Alan   : Rewritten from original code of sciblk4.c
 * of scicos 2.7.
 *
 */

/* prototype */
void sciblk4(Blocks,flag)
     scicos_block *Blocks;
     integer flag;
{
  /*counter and address variable declaration*/
  int i,j,topsave;
  int ierr = 0;

  int *header,ne1;
  double *le111;

  int *il_xd, *il_res, *il_out, *il_outptr, *il_z, *il_x;
  int *il_mode, *il_evout, *il_g;

  /* variable for output typed port */
  int nout;

  /* set number of left and right hand side parameters */
  int mlhs=1,mrhs=2;

  /* Save Top counter */
  topsave=Top;

  /****************************
   * create scilab tlist Blocks
   ****************************/
  if((createblklist(&Blocks[0], &ierr,(i=-1)))==0) goto err;

  /* * flag * */
  ierr=var2sci(&flag,1,1,80);
  if (ierr!=0) goto err;

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
   case 0 : /* update continuous state */
   {
    if (Blocks[0].nx != 0)
    {
     /* 10 - xd */
     il_xd = (int *) listentry(header,10);
     ierr=sci2var(il_xd,Blocks[0].xd,10); /* double */
     if (ierr!=0) goto err;

     /* 11 - res */
     il_res = (int *) listentry(header,11);
     ierr=sci2var(il_res,Blocks[0].res,10); /* double */
     if (ierr!=0) goto err;
    }
   }
   break;

   case 1 : /* update output state */
   {
    /* 17 - outptr */
    if (Blocks[0].nout!=0) 
    {
     il_out = (int*) listentry(header,17);
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

   case 2 : /* update state */
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
      ierr=sci2var(il_z,Blocks[0].z,10); /* double */
      if (ierr!=0) goto err;
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 9 - x */
     il_x = (int *) listentry(header,9);
     ierr=sci2var(il_x,Blocks[0].x,10); /* double */
     if (ierr!=0) goto err;

     /* 10 - xd */
     il_xd = (int *) listentry(header,10);
     ierr=sci2var(il_xd,Blocks[0].xd,10); /* double */
     if (ierr!=0) goto err;
    }

    /* 31 - mode */
    il_mode = (int *) listentry(header,31);
    ierr=sci2var(il_mode,Blocks[0].mode,80); /* int */
    if (ierr!=0) goto err;
   }
   break;

   case 3 : /* update event output state */
   {
    /* 19 - evout */
    il_evout = (int *) listentry(header,19);
    ierr=sci2var(il_evout,Blocks[0].evout,10); /* double */
    if (ierr!=0) goto err;
   }
   break;

   case 4 : /* state initialisation */
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
      ierr=sci2var(il_z,Blocks[0].z,10); /* double */
      if (ierr!=0) goto err;
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 9 - x */
     il_x = (int *) listentry(header,9);
     ierr=sci2var(il_x,Blocks[0].x,10); /* double */
     if (ierr!=0) goto err;

     /* 10 - xd */
     il_xd = (int *) listentry(header,10);
     ierr=sci2var(il_xd,Blocks[0].xd,10); /* double */
     if (ierr!=0) goto err;
    }
   }
   break;

   case 5 : /* finish */
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
      ierr=sci2var(il_z,Blocks[0].z,10); /* double */
      if (ierr!=0) goto err;
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 9 - x */
     il_x = (int *) listentry(header,9);
     ierr=sci2var(il_x,Blocks[0].x,10); /* double */
     if (ierr!=0) goto err;

     /* 10 - xd */
     il_xd = (int *) listentry(header,10); /* double */
     ierr=sci2var(il_xd,Blocks[0].xd,10);
     if (ierr!=0) goto err;
    }
   }
   break;

   case 6 : /* output state initialisation */
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
      ierr=sci2var(il_z,Blocks[0].z,10); /* double */
      if (ierr!=0) goto err;
     }
    }

    if (Blocks[0].nx != 0)
    {
     /* 9 - x */
     il_x = (int *) listentry(header,9);
     ierr=sci2var(il_x,Blocks[0].x,10); /* double */
     if (ierr!=0) goto err;

     /* 10 - xd */
     il_xd = (int *) listentry(header,10);
     ierr=sci2var(il_xd,Blocks[0].xd,10); /* double */
     if (ierr!=0) goto err;
    }

    /* 17 - outptr */
    if (Blocks[0].nout!=0) 
    {
     il_out = (int *) listentry(header,17);
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

   case 7 : /* */
   {
    /* 10 - xd */
    if (Blocks[0].nx != 0)
    {
     il_xd = (int *) listentry(header,10);
     ierr=sci2var(il_xd,Blocks[0].xd,10); /* double */
     if (ierr!=0) goto err;
    }

    /* 31 - mode */
    il_mode = (int *) listentry(header,31);
    ierr=sci2var(il_mode,Blocks[0].mode,80); /* int */
    if (ierr!=0) goto err;
   }
   break;

   case 9 : /* */
   {
    /* 25 - g */
    il_g = (int *) listentry(header,25);
    ierr=sci2var(il_g,Blocks[0].g,10); /* double */
    if (ierr!=0) goto err;

    /* 31 - mode */
    il_mode = (int *) listentry(header,31);
    ierr=sci2var(il_mode,Blocks[0].mode,80); /* int */
    if (ierr!=0) goto err;
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
     case 1  : Scierror(888,"var2sci : error %d. Stack is full.\n",ierr);
               break;

     case 2  : Scierror(888,"var2sci : error %d. No more space on the stack for new data.\n",ierr);
               break;

     default : Scierror(888,"var2sci : error %d. Undefined error.\n",ierr);
               break;
    }
   }
   else /*sci2var error*/
   {
    switch (ierr)
    {
     case 1001  : Scierror(888,"sci2var : error %d. Only integer or double object are accepted.\n",ierr);
                  break;

     case 1002  : Scierror(888,"sci2var : error %d. Bad double object sub_type.\n",ierr);
                  break;

     case 1003  : Scierror(888,"sci2var : error %d. Bad integer object sub_type.\n",ierr);
                  break;

     case 1004  : Scierror(888,"sci2var : error %d. A type of a scilab object has changed.\n",ierr);
                  break;

     default    : Scierror(888,"sci2var : error %d. Undefined error.\n",ierr);
                  break;
    }
   }
  }
  set_block_error(-1);
}


/* sci2var function to convert scilab object
 * to an array of scicos blocks.
 *
 * Input parameters :
 * *x      : void ptr of scilab object.
 * *y      : void ptr of scicos blocks array.
 * typ_var : integer, type of scicos data :
 *           10  : double real
 *           11  : double complex
 *           80  : int
 *           81  : int8
 *           82  : int16
 *           84  : int32
 *           800 : uint
 *           811 : uint8
 *           812 : uint16
 *           814 : uint32
 *
 * Output parameters : int (>1000), error flag 
 *                     (0 if no error)
 *
 * 15/06/06, Alan   : Initial version.
 *
 */

/* prototype */
int sci2var(void *x,void *y, int typ_var)
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
  double *ptr_d, *y_d;
  char *ptr_c, *y_c;
  unsigned char *ptr_uc, *y_uc;
  short *ptr_s, *y_s;
  unsigned short *ptr_us, *y_us;
  int *ptr_i, *y_i;
  unsigned int *ptr_ui,*y_ui;
  long *ptr_l,*y_l;
  unsigned long *ptr_ul,*y_ul;

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
           if ((sub_typ==0)&(typ_var!=10))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==1)&(typ_var!=11))
           {
            err = 1004;
            return err;
           }

           /*copy double matrix*/
           switch (sub_typ)
           {
            case 0 : nm=n*m;
                     ptr_d = (double *) (header+4);
                     y_d = (double *) y;
                     C2F(unsfdcopy)(&nm,ptr_d,(i=-1,&i),y_d,(j=-1,&j));
                     break;

            case 1 : nm=2*n*m;
                     ptr_d = (double *) (header+4);
                     y_d = (double *) y;
                     C2F(unsfdcopy)(&nm,ptr_d,(i=-1,&i),y_d,(j=-1,&j));
                     break;
           }
          }
          break;

  case 8 :
          {/*check type of integer matrix*/
           if ((sub_typ!=1)  & (sub_typ!=2) &(sub_typ!=4) /
               (sub_typ!=11) & (sub_typ!=12)&(sub_typ!=14))
           {
            err = 1003;
            return err;
           }
           if ((sub_typ==1)&(typ_var!=81))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==2)&(typ_var!=82))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==4)&(typ_var!=80)&(typ_var!=84))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==11)&(typ_var!=811))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==12)&(typ_var!=812))
           {
            err = 1004;
            return err;
           }
           if ((sub_typ==14)&(typ_var!=800)&(typ_var!=814))
           {
            err = 1004;
            return err;
           }

           /*copy integer matrix*/
           switch (typ_var)
           {
            case 80  : ptr_i = (int *) (header+4);
                       y_i = (int *) y;
                       for(i=0;i<n*m;i++) y_i[i] = ptr_i[i];
                       break;

            case 81  : ptr_c = (char *) (header+4);
                       y_c = (char *) y;
                       for(i=0;i<n*m;i++) y_c[i] = ptr_c[i];
                       break;

            case 82  : ptr_s = (short *) (header+4);
                       y_s = (short *) y;
                       for(i=0;i<n*m;i++) y_s[i] = ptr_s[i];
                       break;

            case 84  : ptr_l = (long *) (header+4);
                       y_l = (long *) y;
                       for(i=0;i<n*m;i++) y_l[i] = ptr_l[i];
                       break;

            case 800 : ptr_ui = (unsigned int *) (header+4);
                       y_ui = (unsigned int *) y;
                       for(i=0;i<n*m;i++) y_ui[i] = ptr_ui[i];
                       break;

            case 811 : ptr_uc = (unsigned char *) (header+4);
                       y_uc = (unsigned char *) y;
                       for(i=0;i<n*m;i++) y_uc[i] = ptr_uc[i];
                       break;

            case 812 : ptr_us = (unsigned short *) (header+4);
                       y_us = (unsigned short *) y;
                       for(i=0;i<n*m;i++) y_us[i] = ptr_us[i];
                       break;

            case 814 : ptr_ul = (unsigned long *) (header+4);
                       y_ul = (unsigned long *) y;
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
