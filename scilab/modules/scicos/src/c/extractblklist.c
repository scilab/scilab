/*  Scicos
*
*  Copyright (C) INRIA - Alan LAYEC
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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h" /* C2F */
#include "extractblklist.h"
#include "MlistGetFieldNumber.h"
#include "cvstr.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
/*--------------------------------------------------------------------------*/
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
	if (n==-1) 
	{
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
	// pointer value is stored on double values, restoring as a copy will take
	// care of alignement issues and pointer size issues.
	memcpy(&Block->funpt, &ilh[4], sizeof(voidg));

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
	if (n==-1) 
	{
		*ierr = 98;
		return 0;
	}
	ilh           = (int *) (listentry(il,n));
	mh            = ilh[1];
	nh            = ilh[2];
	Block->scsptr = (int) *((double *)(&ilh[4]));

	/* 6 - nz */
	n            = MlistGetFieldNumber(il, "nz");
	if (n==-1) 
	{
		*ierr = 98;
		return 0;
	}
	ilh           = (int *) (listentry(il,n));
	mh            = ilh[1];
	nh            = ilh[2];
	Block->nz     = (int) *((double *)(&ilh[4]));

	/* 7 - z */
	n            = MlistGetFieldNumber(il, "z");
	if (n==-1) 
	{
		*ierr = 98;
		return 0;
	}
	ilh           = (int *) (listentry(il,n));
	mh            = ilh[1];
	nh            = ilh[2];
	Block->z      = NULL;
	if (mh*nh!=0) 
	{
		if ((Block->z=(double *) MALLOC(Block->nz*sizeof(double)))==NULL) 
		{
			*ierr = -39;
			return 0;
		}
		for(j=0;j<Block->nz;j++) 
		{
			Block->z[j]=*((double *)(&ilh[4]) + j);
		}
	}

	/* 8 - noz */
	n            = MlistGetFieldNumber(il, "noz");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
		FREE(Block->z);
		*ierr = 98;
		return 0;
	}
	ilh           = (int *) (listentry(il,n));
	mh            = ilh[1];
	nh            = ilh[2];
	Block->ozsz   = NULL;
	if (mh*nh!=0) 
	{
		if ((Block->ozsz=(int *) MALLOC(2*Block->noz*sizeof(int)))==NULL) 
		{
			FREE(Block->z);
			*ierr = -39;
			return 0;
		}
	}

	for(j=0;j<2*Block->noz;j++) 
	{
		Block->ozsz[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 10 - oztyp */
	n            = MlistGetFieldNumber(il, "oztyp");
	if (n==-1) 
	{
		FREE(Block->z);
		FREE(Block->ozsz);
		*ierr = 98;
		return 0;
	}
	ilh           = (int *) (listentry(il,n));
	mh            = ilh[1];
	nh            = ilh[2];
	Block->oztyp  = NULL;
	if (mh*nh!=0) 
	{
		if ((Block->oztyp=(int *) MALLOC(Block->noz*sizeof(int)))==NULL) 
		{
			FREE(Block->z);
			FREE(Block->ozsz);
			*ierr = -39;
			return 0;
		}
	}
	for(j=0;j<Block->noz;j++) 
	{
		Block->oztyp[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 11 - oz */
	n            = MlistGetFieldNumber(il, "oz");
	if (n==-1) 
	{
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
		if ((Block->ozptr=(void **) MALLOC(Block->noz*sizeof(void *)))==NULL) 
		{
			FREE(Block->z);
			FREE(Block->ozsz);
			FREE(Block->oztyp);
			*ierr = -39;
			return 0;
		}

		for(i=0;i<Block->noz;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block->ozptr[i] = NULL;
			switch(Block->oztyp[i])
			{
			case 10  : if ((Block->ozptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 11  : if ((Block->ozptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 84  : if ((Block->ozptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					   {
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

			case 82  : if ((Block->ozptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					   {
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

			case 81  : if ((Block->ozptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) 
					   {
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

			case 814 : if ((Block->ozptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					   {
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

			case 812 : if ((Block->ozptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					   {
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

			case 811 : if ((Block->ozptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					   {
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->x=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) 
		{
			FREE(Block->z);
			FREE(Block->ozsz);
			FREE(Block->oztyp);
			for(j=0;j<Block->noz;j++) FREE(Block->ozptr[j]);
			FREE(Block->ozptr);
			*ierr = -39;
			return 0;
		}
	}
	for(j=0;j<(mh*nh);j++) 
	{
		Block->x[j]=*((double *)(&ilh[4]) + j);
	}

	/* 14 - xd */
	n            = MlistGetFieldNumber(il, "xd");
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->xd=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) 
		{
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
	for(j=0;j<(mh*nh);j++) 
	{
		Block->xd[j]=*((double *)(&ilh[4]) + j);
	}

	/* 15 - res */
	n            = MlistGetFieldNumber(il, "res");
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->res=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) 
		{
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

	for(j=0;j<(mh*nh);j++) 
	{
		Block->res[j]=*((double *)(&ilh[4]) + j);
	}

	/* 16 - nin */
	n            = MlistGetFieldNumber(il, "nin");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->insz=(int *) MALLOC(3*Block->nin*sizeof(int)))==NULL) 
		{
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
	for(j=0;j<3*Block->nin;j++) 
	{
		Block->insz[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 18 - inptr */
	n            = MlistGetFieldNumber(il, "inptr");
	if (n==-1) 
	{
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
	if (Block->nin!=0) 
	{
		if ((Block->inptr=(void **) MALLOC(Block->nin*sizeof(void *)))==NULL) 
		{
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
		for(i=0;i<Block->nin;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block->inptr[i] = NULL;
			switch(Block->insz[2*Block->nin+i])
			{
			case 10  : if ((Block->inptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 11  : if ((Block->inptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 84  : if ((Block->inptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					   {
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

			case 82  : if ((Block->inptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					   {
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

			case 81  : if ((Block->inptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL)
					   {
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

			case 814 : if ((Block->inptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					   {
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

			case 812 : if ((Block->inptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					   {
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

			case 811 : if ((Block->inptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					   {
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->outsz=(int *) MALLOC(3*Block->nout*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<3*Block->nout;j++) 
	{
		Block->outsz[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 21 - outptr */
	n            = MlistGetFieldNumber(il, "outptr");
	if (n==-1) 
	{
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
	if (Block->nout!=0) 
	{
		if ((Block->outptr=(void **) MALLOC(Block->nout*sizeof(void *)))==NULL) 
		{
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
		for(i=0;i<Block->nout;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block->outptr[i] = NULL;
			switch(Block->outsz[2*Block->nout+i])
			{
			case 10  : if ((Block->outptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 11  : if ((Block->outptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 84  : if ((Block->outptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					   {
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

			case 82  : if ((Block->outptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					   {
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

			case 81  : if ((Block->outptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) 
					   {
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

			case 814 : if ((Block->outptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					   {
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

			case 812 : if ((Block->outptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					   {
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

			case 811 : if ((Block->outptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					   {
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->evout=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) 
		{
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

	for(j=0;j<(mh*nh);j++) 
	{
		Block->evout[j]=*((double *)(&ilh[4]) + j);
	}

	/* 24 - nrpar */
	n            = MlistGetFieldNumber(il, "nrpar");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh * nh != 0) 
	{
		if ((Block->rpar=(double *) MALLOC((mh*nh)*sizeof(double))) == NULL) 
		{
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

	for(j = 0; j < (mh*nh);j++) 
	{
		Block->rpar[j] = *((double *)(&ilh[4]) + j);
	}

	/* 26 - nipar */
	n            = MlistGetFieldNumber(il, "nipar");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->ipar=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) 
		{
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
	for(j=0;j<(mh*nh);j++) 
	{
		Block->ipar[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 28 - nopar */
	n            = MlistGetFieldNumber(il, "nopar");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->oparsz=(int *) MALLOC(2*Block->nopar*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<2*Block->nopar;j++) 
	{
		Block->oparsz[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 30 - opartyp */
	n            = MlistGetFieldNumber(il, "opartyp");
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->opartyp=(int *) MALLOC(Block->nopar*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<Block->nopar;j++) 
	{
		Block->opartyp[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 31 - opar */
	n            = MlistGetFieldNumber(il, "opar");
	if (n==-1) 
	{
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
	if (Block->nopar!=0) 
	{
		if ((Block->oparptr=(void **) MALLOC(Block->nopar*sizeof(void *)))==NULL) 
		{
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

		for(i=0;i<Block->nopar;i++) 
		{
			ilh2 = (int *) (listentry(ilh,i+1));
			mh2  = ilh2[1];
			nh2  = ilh2[2];
			Block->oparptr[i] = NULL;
			switch(Block->opartyp[i])
			{
			case 10  : if ((Block->oparptr[i]=(double *) MALLOC(mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 11  : if ((Block->oparptr[i]=(double *) MALLOC(2*mh2*nh2*sizeof(double)))==NULL) 
					   {
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

			case 84  : if ((Block->oparptr[i]=(long *) MALLOC(mh2*nh2*sizeof(long)))==NULL) 
					   {
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

			case 82  : if ((Block->oparptr[i]=(short *) MALLOC(mh2*nh2*sizeof(short)))==NULL) 
					   {
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

			case 81  : if ((Block->oparptr[i]=(char *) MALLOC(mh2*nh2*sizeof(char)))==NULL) 
					   {
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

			case 814 : if ((Block->oparptr[i]=(unsigned long *) MALLOC(mh2*nh2*sizeof(unsigned long)))==NULL) 
					   {
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

			case 812 : if ((Block->oparptr[i]=(unsigned short *) MALLOC(mh2*nh2*sizeof(unsigned short)))==NULL) 
					   {
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

			case 811 : if ((Block->oparptr[i]=(unsigned char *) MALLOC(mh2*nh2*sizeof(unsigned char)))==NULL) 
					   {
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->g=(double *) MALLOC((mh*nh)*sizeof(double)))==NULL) 
		{
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
	
	for(j=0;j<(mh*nh);j++) 
	{
		Block->g[j]=*((double *)(&ilh[4]) + j);
	}

	/* 34 - ztyp */
	n            = MlistGetFieldNumber(il, "ztyp");
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->jroot=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<(mh*nh);j++) 
	{
		Block->jroot[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 36 - label */
	n            = MlistGetFieldNumber(il, "label");
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		len_str  = ilh[5]-1;
		if (len_str!=0) 
		{
			if ((Block->label=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) 
			{
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (n==-1) 
	{
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
	if (mh*nh!=0) 
	{
		if ((Block->mode=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<(mh*nh);j++) 
	{
		Block->mode[j]=(int) *((double *)(&ilh[4]) + j);
	}

	/* 40 - xprop */
	n            = MlistGetFieldNumber(il, "xprop");
	if (n==-1) 
	{
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
		if ((Block->xprop=(int *) MALLOC((mh*nh)*sizeof(int)))==NULL) 
		{
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

	for(j=0;j<(mh*nh);j++) 
	{
		Block->xprop[j]=(int) *((double *)(&ilh[4]) + j);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
