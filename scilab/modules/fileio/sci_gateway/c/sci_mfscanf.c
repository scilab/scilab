/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "stack-c.h"
#include "do_xxprintf.h"
#include "do_xxscanf.h"
#include "fileio.h"
#include "gw_fileio.h"
#include "filesmanagement.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int int_objfscanf(char *fname,unsigned long fname_len)
{
	static int l1, m1, n1,l2,m2,n2,iarg,maxrow,nrow,rowcount,ncol;
	FILE  *f;
	int args,retval,err;
	int retval_s=0;
	entry *data=NULL;
	long int pos;
	int param1=0;

	rec_entry buf[MAXSCAN];
	sfdir  type[MAXSCAN],type_s[MAXSCAN];

	Nbvars = 0;
	CheckRhs(2,3);

	if (Rhs==3)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		if (m1*n1 != 1 )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: Scalar expected.\n"),fname,1);
			return 0;
		}
		iarg=2;
		maxrow=*istk(l1);
	}
	else
	{
		iarg=1;
		maxrow=1;
	}

	GetRhsVar(iarg,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	GetRhsVar(iarg+1,STRING_DATATYPE,&m2,&n2,&l2);/* format */

	param1 = *istk(l1);
	StringConvert(cstk(l2));  /* conversion */

	switch (param1)
	{
	case 0:
		// stderr
		f = (FILE *)0;
		break;
	case 5:
		f = stdin;
		break;
	case 6:
		// stdout
		f = (FILE *)0;
		break;
	default:
		f = GetFileOpenedInScilab(param1);
		break;
	}

	if (f == (FILE *)0)
	{
		Scierror(999,_("%s: Wrong file descriptor: %d.\n"),fname,param1);
		return 0;
	}

	nrow = maxrow;
	rowcount = -1;

	while (1)
	{
		rowcount++;
		if ((maxrow >= 0) && (rowcount >= maxrow)) break;
		args = Rhs; /* args set to Rhs on entry */
		pos=ftell(f);
		if ( do_xxscanf("fscanf",f,cstk(l2),&args,(char *)0,&retval,buf,type) < 0 )  return 0;
		if ( retval == EOF)
		{
			/*
			Scierror(999,"Error: in %s: end of file reached\n",fname);
			*/
			break;
		}
		if ((err=Store_Scan(&nrow,&ncol,type_s,type,&retval,&retval_s,buf,&data,rowcount,args)) <0 )
		{
			switch (err)
			{
			case MISMATCH:
				if (maxrow>=0)
				{
					Free_Scan(rowcount,ncol,type_s,&data);
					Scierror(999,_("%s: Data mismatch.\n"),fname);
					return 0;
				}
				fseek(f,pos,SEEK_SET);
				break;

			case MEM_LACK:
				Free_Scan(rowcount,ncol,type_s,&data);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
				break;
			}
			if (err==MISMATCH) break;
		}
	} /* while */

	/* create Scilab variable with each column of data */
	err=Sci_Store(rowcount,ncol,data,type_s,retval_s);
	Free_Scan(rowcount,ncol,type_s,&data);
	if (err==MEM_LACK) { Scierror(999,_("%s: No more memory.\n"),fname);}
	return 0;
}
/*--------------------------------------------------------------------------*/
