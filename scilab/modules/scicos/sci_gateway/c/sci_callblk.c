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
#include "scicos_block4.h"
#include "Scierror.h"
#include "localization.h"
#include "cvstr.h"
#include "createblklist.h"
#include "extractblklist.h"
#include "scicos.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
/*--------------------------------------------------------------------------*/
int sci_callblk(char *fname,unsigned long fname_len)
{
	/* auxilary variables -dimension and address-
	* for scilab stack variables
	*/
	int *il1 = NULL;
	int m1 = 0, n1 = 0;
	int *il2_1 = NULL;
	int m2_1 = 0, n2_1 = 0;
	int *il2 = NULL;
	int m2 = 0, n2 = 0;
	int *il3 = NULL;
	int m3 = 0, n3 = 0;

	/* local variable */
	int len_str = 0;
	char *str = NULL;
	int ierr = 0, ret = 0;
	int j = 0;
	int TopSave = 0;
	int l_tmp = 0;

	/* length of the scilab list scicos struct */
	int nblklst = 40;

	/* variable for callf */
	scicos_flag flag = 0;
	double t = 0.;
	scicos_block Block;

	/* check number of rhs/lhs param */
	CheckRhs(3,3);
	CheckLhs(1,1);

	/* check rhs 1 (input scilab structure) */
	il1 = (int *) GetData(1);
	m1  = il1[1];
	n1  = il1[2];
	if (il1[0]!=16) 
	{
		Scierror(888, _("%s : First argument must be a scicos_block typed list.\n"),fname);
		return 0;
	}

	il2_1 = (int *) (listentry(il1,1));
	m2_1  = il2_1[1];
	n2_1  = il2_1[2];
	if ((il2_1[0]!=10) || ((m2_1*n2_1)!=nblklst)) 
	{
		Scierror(888, _("%s : First argument must be a valid scicos_block typed list.\n"),fname);
		return 0;
	}
	len_str = il2_1[5]-1;
	if (len_str!=0) 
	{
		if ((str=(char *) MALLOC((len_str+1)*sizeof(char)))==NULL) 
		{
			Scierror(888,_("%s: Memory allocation error.\n"),fname);
			return 0;
		}
		str[len_str] = '\0';
		C2F(cvstr)(&len_str,&il2_1[5+nblklst],str,(j=1,&j),len_str);
		ret = strcmp("scicos_block", str);
		FREE(str);
		if (ret!=0) 
		{
			Scierror(888,_("%s : First argument must be a valid scicos_block typed list.\n"),fname);
			return 0;
		}
	}
	else 
	{
		Scierror(888,_("%s : First argument must be a valid scicos_block typed list.\n"),fname);
		return 0;
	}

	/* convert scilab scicos struct to a C scicos struct */
	ret = extractblklist(il1, &Block, &ierr);

	/* error table */
	switch (ierr)
	{
	case -39 : Scierror(888,_("%s: Memory allocation error.\n"),fname);
		break;

	case 98  : Scierror(888,_("%s : First argument must be a valid scicos_block typed list.\n"),fname);
		break;

	default: break;
	}

	if (ierr!=0) 
	{
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
	if ((il2[0]!=1) || (m2*n2!=1))
	{
		Scierror(888,_("%s : Second argument must be scalar.\n"),fname);
		return 0;
	}
	flag=(scicos_flag) *((double *)(&il2[4]));

	/* check rhs 3 (time) */
	il3 = (int *) GetData(3);
	m3  = il3[1];
	n3  = il3[2];

	if ((il3[0]!=1) || (m3*n3!=1))
	{
		Scierror(888,_("%s : Third argument must be scalar.\n"),fname);
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

	CreateVar(4,TYPED_LIST_DATATYPE,&nblklst,(j=1,&j),&l_tmp);
	LhsVar(1) = 4;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
