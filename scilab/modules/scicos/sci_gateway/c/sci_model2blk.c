/*  Scicos
*
*  Copyright (C) INRIA - 
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
#include <stdio.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "scicos_block4.h"
#include "MALLOC.h"
#include "cvstr.h"
#include "sciblk4.h"
#include "sciblk2.h"
#include "MlistGetFieldNumber.h"
#include "dynamic_link.h"
#include "createblklist.h"
#include "scicos.h"
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
extern int C2F(funnum) (char *fname);
extern int C2F(namstr)();
extern void  F2C(sciblk)();
/*--------------------------------------------------------------------------*/
extern int ntabsim;
extern OpTab tabsim[];
/*--------------------------------------------------------------------------*/
/* model2blk Build a scicos_block structure from
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
	int m1 = 0,n1 = 0;
	int *il1 = NULL;
	int n = 0;
	int mh = 0,nh = 0;
	int *ilh = NULL;
	int mh2 = 0,nh2 = 0;
	int *ilh2 = NULL;
	int mh3 = 0,nh3 = 0;
	int *ilh3 = NULL;
	int ierr = 0;
	int TopSave = 0;
	int i = 0,j = 0;
	int l_tmp = 0;
	double type = 0.0;
	int len_str = 0;
	int lfunpt = 0;
	int typfsim = 0;
	int ifun = 0;
	int *il_sim = NULL;

	int id[nsiz];

	double *ptr_d = NULL;
	char *ptr_c = NULL;
	unsigned char *ptr_uc = NULL;
	short *ptr_s = NULL;
	unsigned short *ptr_us = NULL;
	long *ptr_l = NULL;
	unsigned long *ptr_ul = NULL;

	scicos_block Block;

	int nblklst = 40;

	/* check size of rhs/lhs parameters */
	CheckRhs(1,1);
	CheckLhs(1,1);

	il1 = (int *) GetData(1);
	m1  = il1[1];
	n1  = il1[2];

	/* check for a tlist */
	if (il1[0] != sci_mlist) 
	{
		Scierror(888,_("%s : First argument must be a Typed list.\n"),fname);
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
		(ilh[mh*nh+9]!=21)) 
	{
			Scierror(888,_("%s : First argument must be a scicos model.\n"),fname);
			return 0;
	}

	/* TODO */
	/* 2 : model.sim  */
	n            = MlistGetFieldNumber(il1, "sim");
	ilh          = (int *) (listentry(il1,n));
	mh           = ilh[1];
	nh           = ilh[2];
	if (ilh[0] == sci_list) 
	{ 
		/* sim  is a list */
		ilh2 = (int *) (listentry(ilh,2));
		Block.type = (int) *((double *) (&ilh2[4]));
		ilh2 = (int *) (listentry(ilh,1));
		typfsim = ilh2[0]; /* typfsim  the name the comput funct */
		il_sim = ilh2;
	}
	else 
	{
		Block.type = 0;
		typfsim = ilh[0];
		il_sim = ilh;
	}
	/* check if typfsim is a scilab function */
	if ((typfsim == sci_u_function) || (typfsim == sci_c_function)) 
	{
		/* TODO */
		lfunpt = -1;
	}
	/* check if typfsim is a string */
	else if(typfsim == sci_strings)
	{
		len_str = il_sim[5]-1;
		C2F(cha1).buf[0]=' ';
		C2F(cvstr)(&len_str,&il_sim[6],&C2F(cha1).buf[0],(j=1,&j),len_str);
		C2F(cha1).buf[len_str] = '\0';
		/* search fun ptr of the comput func in the scilab func table */
		ifun=C2F(funnum)(C2F(cha1).buf);
		if (ifun>0) lfunpt=ifun;
		else 
		{
			C2F(namstr)(id,&il_sim[6],&len_str,(j=0,&j));
			C2F(com).fin=0;
			C2F(funs)(id);
			if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt=-*Lstk(C2F(com).fin);
			else
			{
				lfunpt=0;
				Scierror(888, _("%s : unknown block : %s\n"),fname,C2F(cha1).buf);
				return 0;
			}
		}
	}
	/* else {
	* error
	}*/
	/* comput func is a scilab function */
	if (lfunpt<0) 
	{
		switch (Block.type) 
		{
	case 0:
		Block.funpt=F2C(sciblk);
		break;
	case 1:
		Scierror(888, _("%s : type 1 function not allowed for scilab blocks\n"),fname);
		return 0;
	case 2:
		Scierror(888, _("%s : type 2 function not allowed for scilab blocks\n"),fname);
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
		Scierror(888,_("%s : Undefined Function type\n"),fname);
		return 0;
		}
		Block.scsptr=-lfunpt;
	}
	else if (lfunpt<=ntabsim) 
	{
		Block.funpt=*(tabsim[lfunpt-1].fonc);
		Block.scsptr=0;
	}
	else 
	{
		lfunpt -= (ntabsim+1);
		GetDynFunc(lfunpt,&Block.funpt);
		if (Block.funpt == (voidf) 0) 
		{
			Scierror(888,_("%s : Function not found\n"),fname);
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
	if (mh*nh!=0) 
	{
		/* check value of in */
		for (i=0;i<Block.nin;i++) 
		{
			if ((*((double *)(&ilh[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Size. in(%d)=%d. Please adjust your model.\n"),\
					fname,i+1,(int)(*((double *)(&ilh[4]) + i)));
				return 0;
			}
		}
		/* alloc insz */
		if ((Block.insz=(int *) MALLOC(Block.nin*3*sizeof(int)))==NULL) 
		{
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}
		/* alloc inptr */
		if ((Block.inptr=(void **) MALLOC(Block.nin*sizeof(void *)))==NULL) 
		{
			FREE(Block.insz);
			Scierror(888, _("%s : Allocation error.\n"),fname);
			return 0;
		}
		/* 4 : model.in2  */
		n    = MlistGetFieldNumber(il1, "in2");
		ilh2 = (int *) (listentry(il1,n));
		mh2  = ilh2[1];
		nh2  = ilh2[2];
		/* check value of in2 */
		for (i=0;i<(mh2*nh2);i++) 
		{
			if ((*((double *)(&ilh2[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Size. in2(%d)=%d. Please adjust your model.\n"),\
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
		for (i=0;i<(mh3*nh3);i++) 
		{
			if ((*((double *)(&ilh3[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Type. intyp(%d)=%d. Please adjust your model.\n"),\
					fname,i+1,(int)(*((double *)(&ilh3[4]) + i)));
				FREE(Block.insz);
				FREE(Block.inptr);
				return 0;
			}
		}
		if (((mh*nh)==(mh2*nh2)) && (((mh*nh)==(mh3*nh3)))) 
		{
			for(i=0;i<Block.nin;i++) 
			{
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
		else 
		{
			for(i=0;i<Block.nin;i++) 
			{
				Block.insz[i]=(int) *((double *)(&ilh[4]) + i);
				Block.insz[Block.nin+i]=1;
				Block.insz[2*Block.nin+i]=10;
			}
		}

		for(i=0;i<Block.nin;i++) 
		{
			switch(Block.insz[2*Block.nin+i])
			{
			case 10  : 
				if ((Block.inptr[i]=(double *) MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(double)))==NULL) 
				   {
					   for(j=0;j<i;j++) FREE(Block.inptr[j]);
					   FREE(Block.inptr);
					   FREE(Block.insz);
					   Scierror(888,_("%s : Allocation error.\n"),fname);
					   return 0;
				   }
				 ptr_d = (double *) Block.inptr[i];
				 for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
				 {
				  ptr_d[j]=0.;
				 }
				 break;
			case 11  : if ((Block.inptr[i]=(double *) \
						   MALLOC(2*Block.insz[i]*Block.insz[Block.nin+i]*sizeof(double)))==NULL) 
					   {
						   for(j=0;j<i;j++) FREE(Block.inptr[j]);
						   FREE(Block.inptr);
						   FREE(Block.insz);
						   Scierror(888,_("%s : Allocation error.\n"),fname);
						   return 0;
					   }
					   ptr_d = (double *) Block.inptr[i];
					   for(j=0;j<2*Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_d[j]=0.;
					   }
					   break;
			case 84  : if ((Block.inptr[i]=(long *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(long)))==NULL) 
					   {
						   for(j=0;j<i;j++) FREE(Block.inptr[j]);
						   FREE(Block.inptr);
						   FREE(Block.insz);
						   Scierror(888,_("%s : Allocation error.\n"),fname);
						   return 0;
					   }
					   ptr_l = (long *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_l[j]=0;
					   }
					   break;
			case 82  : if ((Block.inptr[i]=(short *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(short)))==NULL) 
					   {
							   for(j=0;j<i;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   Scierror(888, _("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_s = (short *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_s[j]=0;
					   }
					   break;
			case 81  : if ((Block.inptr[i]=(char *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(char)))==NULL) 
					   {
							   for(j=0;j<i;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_c = (char *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_c[j]=0;
					   }
					   break;
			case 814 : if ((Block.inptr[i]=(unsigned long *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned long)))==NULL) 
					   {
							   for(j=0;j<i;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_ul = (unsigned long *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_ul[j]=0;
					   }
					   break;
			case 812 : if ((Block.inptr[i]=(unsigned short *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned short)))==NULL) 
					   {
							   for(j=0;j<i;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_us = (unsigned short *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
						   ptr_us[j]=0;
					   }
					   break;
			case 811 : if ((Block.inptr[i]=(unsigned char *) \
						   MALLOC(Block.insz[i]*Block.insz[Block.nin+i]*sizeof(unsigned char)))==NULL) 
					   {
							   for(j=0;j<i;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_uc = (unsigned char *) Block.inptr[i];
					   for(j=0;j<Block.insz[i]*Block.insz[Block.nin+i];j++) 
					   {
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
	if (mh*nh!=0) 
	{
		/* check value of out */
		for (i=0;i<Block.nout;i++) 
		{
			if ((*((double *)(&ilh[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Size. out(%d)=%d. Please adjust your model.\n"),\
					fname,i+1,(int)(*((double *)(&ilh[4]) + i)));
				for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
				FREE(Block.inptr);
				FREE(Block.insz);
				return 0;
			}
		}
		/* alloc outsz */
		if ((Block.outsz=(int *) MALLOC(Block.nout*3*sizeof(int)))==NULL) {
			Scierror(888,_("%s : Allocation error.\n"),fname);
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}
		/* 7 : model.out2  */
		n    = MlistGetFieldNumber(il1, "out2");
		ilh2 = (int *) (listentry(il1,n));
		mh2  = ilh2[1];
		nh2  = ilh2[2];
		/* check value of out2 */
		for (i=0;i<(mh2*nh2);i++) {
			if ((*((double *)(&ilh2[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Size. out2(%d)=%d. Please adjust your model.\n"),\
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
			if ((*((double *)(&ilh3[4]) + i)) <= 0.) 
			{
				Scierror(888,_("%s : Undetermined Type. outtyp(%d)=%d. Please adjust your model.\n"),\
					fname,i+1,(int)(*((double *)(&ilh3[4]) + i)));
				FREE(Block.insz);
				FREE(Block.inptr);
				FREE(Block.outptr);
				FREE(Block.outsz);
				return 0;
			}
		}
		if (((mh*nh)==(mh2*nh2)) && (((mh*nh)==(mh3*nh3)))) 
		{
			for(i=0;i<Block.nout;i++) 
			{
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
		else 
		{
			for(i=0;i<Block.nout;i++) 
			{
				Block.outsz[i]=(int) *((double *)(&ilh[4]) + i);
				Block.outsz[Block.nout+i]=1;
				Block.outsz[2*Block.nout+i]=10;
			}
		}
		for(i=0;i<Block.nout;i++) 
		{
			switch(Block.outsz[2*Block.nout+i])
			{
			case 10  : if ((Block.outptr[i]=(double *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(double)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_d = (double *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_d[j]=0.;
					   }
					   break;
			case 11  : if ((Block.outptr[i]=(double *) \
						   MALLOC(2*Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(double)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_d = (double *) Block.outptr[i];
					   for(j=0;j<2*Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_d[j]=0.;
					   }
					   break;
			case 84  : if ((Block.outptr[i]=(long *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(long)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_l = (long *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_l[j]=0;
					   }
					   break;
			case 82  : if ((Block.outptr[i]=(short *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(short)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_s = (short *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_s[j]=0;
					   }
					   break;
			case 81  : if ((Block.outptr[i]=(char *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(char)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_c = (char *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
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
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_ul = (unsigned long *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_ul[j]=0;
					   }
					   break;
			case 812 : if ((Block.outptr[i]=(unsigned short *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(unsigned short)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888,_("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_us = (unsigned short *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
						   ptr_us[j]=0;
					   }
					   break;
			case 811 : if ((Block.outptr[i]=(unsigned char *) \
						   MALLOC(Block.outsz[i]*Block.outsz[Block.nout+i]*sizeof(unsigned char)))==NULL) 
					   {
							   for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
							   FREE(Block.inptr);
							   FREE(Block.insz);
							   for(j=0;j<i;j++) FREE(Block.outptr[j]);
							   FREE(Block.outptr);
							   FREE(Block.outsz);
							   Scierror(888, _("%s : Allocation error.\n"),fname);
							   return 0;
					   }
					   ptr_uc = (unsigned char *) Block.outptr[i];
					   for(j=0;j<Block.outsz[i]*Block.outsz[Block.nout+i];j++) 
					   {
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
	if (mh*nh!=0) 
	{
		if ((Block.evout=(double *) MALLOC(Block.nevout*sizeof(double)))==NULL) 
		{
			for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
			FREE(Block.inptr);
			FREE(Block.insz);
			for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
			FREE(Block.outptr);
			FREE(Block.outsz);
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}
		n            = MlistGetFieldNumber(il1, "firing");
		ilh2         = (int *) (listentry(il1,n));
		mh2          = ilh2[1];
		nh2          = ilh2[2];
		if ((mh*nh)==(mh2*nh2)) 
		{
			for(j=0;j<Block.nevout;j++) 
			{
				Block.evout[j]=*((double *)(&ilh2[4]) + j);
			}
		}
		else 
		{
			for(j=0;j<Block.nevout;j++) 
			{
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
	if (mh*nh!=0) 
	{
		/* x */
		if ((Block.x=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) 
		{
			for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
			FREE(Block.inptr);
			FREE(Block.insz);
			for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
			FREE(Block.outptr);
			FREE(Block.outsz);
			FREE(Block.evout);
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nx;j++) 
		{
			Block.x[j]=*((double *)(&ilh[4]) + j);
		}

		/* xd */
		if ((Block.xd=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) 
		{
			for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
			FREE(Block.inptr);
			FREE(Block.insz);
			for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
			FREE(Block.outptr);
			FREE(Block.outsz);
			FREE(Block.evout);
			FREE(Block.x);
			Scierror(888, _("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nx;j++) 
		{
			Block.xd[j]=0.;
		}
		/* xprop */
		if ((Block.xprop=(int *) MALLOC(Block.nx*sizeof(int)))==NULL) 
		{
			for(j=0;j<Block.nin;j++) FREE(Block.inptr[j]);
			FREE(Block.inptr);
			FREE(Block.insz);
			for(j=0;j<Block.nout;j++) FREE(Block.outptr[j]);
			FREE(Block.outptr);
			FREE(Block.outsz);
			FREE(Block.evout);
			FREE(Block.x);
			FREE(Block.xd);
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nx;j++) 
		{
			Block.xprop[j]=1;
		}
		/* res */
		/*if (blktyp>10000) {*/
		if ((Block.res=(double *) MALLOC(Block.nx*sizeof(double)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nx;j++) 
		{
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

	if (mh*nh!=0) 
	{
		if ((Block.z=(double *) MALLOC(Block.nz*sizeof(double)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nz;j++) 
		{
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
	if (mh*nh!=0) 
	{
		if ((Block.ozsz=(int *) MALLOC(2*Block.noz*sizeof(int)))==NULL) 
		{
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
			Scierror(888, _("%s : Allocation error.\n"),fname);
			return 0;
		}

		if ((Block.oztyp=(int *) MALLOC(Block.noz*sizeof(int)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		if ((Block.ozptr=(void **) MALLOC(Block.noz*sizeof(void *)))==NULL) 
		{
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
			Scierror(888, _("%s : Allocation error.\n"),fname);
			return 0;
		}

		for (i=0;i<mh*nh;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block.ozsz[i] = mh2;
			Block.ozsz[Block.noz+i] = nh2;
			if (ilh2[0]==1) 
			{
				if (ilh2[3]==0) 
				{
					Block.oztyp[i] = 10;
					if ((Block.ozptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					{
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
						Scierror(888, _("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_d = (double *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_d[j]=*((double *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==1) 
				{
					Block.oztyp[i] = 11;
					if ((Block.ozptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					{
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
						Scierror(888, _("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_d = (double *) Block.ozptr[i];
					for(j=0;j<2*mh2*nh2;j++) 
					{
						ptr_d[j]=*((double *)(&ilh2[4]) + j);
					}
				}
			}
			else if (ilh2[0]==8) 
			{
				if (ilh2[3]==4) 
				{
					Block.oztyp[i] = 84;
					if ((Block.ozptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_l = (long *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_l[j]=*((long *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==2) 
				{
					Block.oztyp[i] = 82;
					if ((Block.ozptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_s = (short *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_s[j]=*((short *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==1) 
				{
					Block.oztyp[i] = 81;
					if ((Block.ozptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_c = (char *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_c[j]=*((char *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==14) 
				{
					Block.oztyp[i] = 814;
					if ((Block.ozptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_ul = (unsigned long *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_ul[j]=*((unsigned long *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==12) 
				{
					Block.oztyp[i] = 812;
					if ((Block.ozptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_us = (unsigned short *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_us[j]=*((unsigned short *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==11) 
				{
					Block.oztyp[i] = 811;
					if ((Block.ozptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_uc = (unsigned char *) Block.ozptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
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
	if (mh*nh!=0) 
	{
		if ((Block.rpar=(double *) MALLOC(Block.nrpar*sizeof(double)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}
		for(j=0;j<Block.nrpar;j++) 
		{
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
	if (mh*nh!=0) 
	{
		if ((Block.ipar=(int *) MALLOC(Block.nipar*sizeof(int)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nipar;j++) 
		{
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
	if (mh*nh!=0) 
	{
		if ((Block.oparsz=(int *) MALLOC(2*Block.nopar*sizeof(int)))==NULL) 
		{
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
			Scierror(888, _("%s : Allocation error.\n"),fname);
			return 0;
		}

		if ((Block.opartyp=(int *) MALLOC(Block.nopar*sizeof(int)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		if ((Block.oparptr=(void **) MALLOC(Block.nopar*sizeof(void *)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for (i=0;i<mh*nh;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block.oparsz[i] = mh2;
			Block.oparsz[Block.nopar+i] = nh2;
			if (ilh2[0]==1) 
			{
				if (ilh2[3]==0) 
				{
					Block.opartyp[i] = 10;
					if ((Block.oparptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_d = (double *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_d[j]=*((double *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==1) 
				{
					Block.opartyp[i] = 11;
					if ((Block.oparptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					{
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
						Scierror(888, _("%s : Allocation error.\n"),fname);
						return 0;
					}

					ptr_d = (double *) Block.oparptr[i];
					for(j=0;j<2*mh2*nh2;j++) 
					{
						ptr_d[j]=*((double *)(&ilh2[4]) + j);
					}
				}
			}
			else if (ilh2[0]==8) 
			{
				if (ilh2[3]==4) 
				{
					Block.opartyp[i] = 84;
					if ((Block.oparptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_l = (long *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_l[j]=*((long *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==2) 
				{
					Block.opartyp[i] = 82;
					if ((Block.oparptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_s = (short *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_s[j]=*((short *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==1) 
				{
					Block.opartyp[i] = 81;
					if ((Block.oparptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_c = (char *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_c[j]=*((char *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==14) 
				{
					Block.opartyp[i] = 814;
					if ((Block.oparptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_ul = (unsigned long *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_ul[j]=*((unsigned long *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==12) 
				{
					Block.opartyp[i] = 812;
					if ((Block.oparptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					{
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
						Scierror(888, _("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_us = (unsigned short *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
						ptr_us[j]=*((unsigned short *)(&ilh2[4]) + j);
					}
				}
				else if (ilh2[3]==11) 
				{
					Block.opartyp[i] = 811;
					if ((Block.oparptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					{
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
						Scierror(888,_("%s : Allocation error.\n"),fname);
						return 0;
					}
					ptr_uc = (unsigned char *) Block.oparptr[i];
					for(j=0;j<mh2*nh2;j++) 
					{
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
	if (mh*nh!=0) 
	{
		len_str  = ilh[5]-1;
		if (len_str!=0) 
		{
			if ((Block.label=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) 
			{
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
				Scierror(888,_("%s : Allocation error.\n"),fname);
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
	if (Block.ng!=0) 
	{
		if ((Block.g=(double *) MALLOC(Block.ng*sizeof(double)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.ng;j++) 
		{
			Block.g[j]=0.;
		}
		if ((Block.jroot=(int *) MALLOC(Block.ng*sizeof(int)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.ng;j++) 
		{
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
	if (Block.nmode!=0) 
	{
		if ((Block.mode=(int *) MALLOC(Block.nmode*sizeof(double)))==NULL) 
		{
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
			Scierror(888,_("%s : Allocation error.\n"),fname);
			return 0;
		}

		for(j=0;j<Block.nmode;j++) 
		{
			Block.mode[j]=0;
		}
	}

	/* work */
	if ((Block.work=(void **) MALLOC(sizeof(void *)))==NULL) 
	{
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
		Scierror(888,_("%s : Allocation error.\n"),fname);
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

	CreateVar(2,TYPED_LIST_DATATYPE,&nblklst,(j=1,&j),&l_tmp);
	LhsVar(1) = 2;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
