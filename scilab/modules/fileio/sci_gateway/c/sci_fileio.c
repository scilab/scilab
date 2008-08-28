/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
#include "gw_fileio.h"
#include "cluni0.h"
#include "mopen.h"
#include "mput.h"
#include "mputstr.h"
#include "mclose.h"
#include "meof.h"
#include "mseek.h"
#include "merror.h"
#include "mtell.h"
#include "mclearerr.h"
#include "localization.h"
#include "mget.h"
#include "mgetstr.h"
#include "Scierror.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static char filename[PATH_MAX];
static int out_n;
static long int lout;
/*--------------------------------------------------------------------------*/
/* SCILAB function : mopen */
/*--------------------------------------------------------------------------*/
int intsmopen(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,l5,err;
	int swap = 1,un=1;
	char *status;

	Nbvars=0;
	CheckRhs(1,3);
	CheckLhs(1,2);
	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	if ( Rhs >= 2)
	{
		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		status = cstk(l2);
	}
	else
	{
		status = "rb";
	}
	if ( Rhs >= 3)
	{
		GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
		swap = *istk(l3);
	}
	CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &un,&un, &l4);
	CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &un,&un, &l5);
	lout=PATH_MAX;
	C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);

	C2F(mopen)(istk(l4),filename,status,&swap,stk(l5),&err);
	if (err >  0)
	{
		if ( Lhs == 1)
		{
			if ( err == 1) {
				Error(66);/* no more logical units */
				return 0;
			}
			else if ( err == 2)
			{
				Scierror(999,_("%s: Cannot open file %s.\n"),fname,filename);
				return 0;
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
		else
		{
			*stk(l5) = - err;
		}
	}
	LhsVar(1)= Rhs+1;
	LhsVar(2)= Rhs+2;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mputstr */
/*--------------------------------------------------------------------------*/
int intsmputstr(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3,err;
	int fd = -1;
	Nbvars=0;
	CheckRhs(1,2);
	CheckLhs(1,1);
	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	if ( Rhs >= 2)
	{
		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		fd = *istk(l2);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &m3,&n3, &l3);
	C2F(mputstr)(&fd,cstk(l1),stk(l3),&err);
	if (err >  0)
	{
		/*sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	};
	LhsVar(1)= Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mclose */
/*--------------------------------------------------------------------------*/
int intsmclose(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,un=1,l2;
	int fd = -1;
	Nbvars=0;
	CheckRhs(0,1);
	CheckLhs(1,1);
	/*  checking variable file */
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd = *istk(l1);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &un,&un, &l2);
	C2F(mclose)(&fd,stk(l2));
	LhsVar(1)= Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mput */
/*--------------------------------------------------------------------------*/
int intsmput(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,m3,n3,l3,err;
	char *type;
	int fd=-1;
	Nbvars=0;
	CheckRhs(1,3);
	CheckLhs(1,1);
	/*  checking variable res */
	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	n1=m1*n1;
	if ( Rhs >= 2)
	{
		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		type = cstk(l2);
	}
	else
	{
		type = LIST_DATATYPE;
	}
	if ( Rhs >= 3)
	{
		GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
		fd = *istk(l3);
	}
	C2F(mput)(&fd,stk(l1),&n1,type,&err);
	if (err >  0)
	{
		/* sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	};
	LhsVar(1)= 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mget */
/*--------------------------------------------------------------------------*/
int intsmget(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,err;
	char *type;
	int fd=-1;
	int n=1,un=1;
	Nbvars=0;
	CheckRhs(1,3);
	CheckLhs(1,1);
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		n  = *istk(l1);
	}
	if ( Rhs >= 2)
	{
		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		type = cstk(l2);
	}
	else
	{
		type = LIST_DATATYPE;
	}
	if ( Rhs >= 3)
	{
		GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
		fd = *istk(l3);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&n,&l4);
	LhsVar(1)= Rhs+1;
	C2F(mget)(&fd,stk(l4),&n,type,&err);
	if (err >  0)
	{
		/*      sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	}
	else if ( err < 0)
	{
		int n5,l5,i;
		/** n contains now the effectively read data **/
		n5 = -err -1;
		if ( n5 < n )
		{
			CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&un,&n5,&l5);
			for ( i=0; i < n5 ; i++) *stk(l5+i) = *stk(l4+i);
			LhsVar(1)= Rhs+2;
		}
	}
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mgetstr */
/*--------------------------------------------------------------------------*/
int intsmgetstr(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,l3,err;
	int fd=-1;
	int n=1,un=1;
	Nbvars=0;
	CheckRhs(1,2);
	CheckLhs(1,1);
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		n  = *istk(l1);
	}
	if ( Rhs >= 2)
	{
		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		fd = *istk(l2);
	}
	CreateVar(Rhs+1,STRING_DATATYPE,&n,&un,&l3);
	C2F(mgetstr1)(&fd,cstk(l3),&n,&err);
	LhsVar(1)=Rhs+1;
	if (err >  0)
	{
		/*      sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	}
	else if ( err < 0)
	{
		int n5,l5;
		/** n contains now the effectively read data **/
		n5 = -err -1;
		if ( n5 < n )
		{
			CreateVar(Rhs+2,STRING_DATATYPE,&un,&n5,&l5);
			strcpy(cstk(l5),cstk(l3));
			LhsVar(1)= Rhs+2;
		}
	}
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : meof */
/*--------------------------------------------------------------------------*/
int intsmeof(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,un=1,lr;
	int fd=-1;
	Nbvars=0;
	CheckRhs(0,1);
	CheckLhs(1,1);
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd  = *istk(l1);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr);
	C2F(meof)(&fd,stk(lr));
	LhsVar(1)= Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mseek */
/*--------------------------------------------------------------------------*/
int intsmseek(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,m2,n2,l2,m3,n3,l3,err;
	int fd=-1;
	char *flag;
	Nbvars=0;
	CheckRhs(1,3);
	CheckLhs(1,1);
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	if ( Rhs >= 2)
	{
		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		fd = *istk(l2);
	}
	if ( Rhs >= 3)
	{
		GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
		flag = cstk(l3);
	}
	else
	{
		flag = "set";
	}
	C2F(mseek)(&fd,istk(l1),flag,&err);
	if (err >  0)
	{
		/* sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	};
	LhsVar(1)=0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mtell */
/*--------------------------------------------------------------------------*/
int intsmtell(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,un=1,l2,err;
	int fd = -1;
	Nbvars=0;
	CheckRhs(0,1);
	CheckLhs(1,1);
	/*  checking variable file */
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd = *istk(l1);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &un,&un, &l2);
	C2F(mtell)(&fd,stk(l2),&err);
	if (err >  0)
	{
		/*sciprint("%s: Internal Error\n",fname);*/
		Error(10000);
		return 0;
	};
	LhsVar(1)= Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : mclearerr */
/*--------------------------------------------------------------------------*/
int intsmclearerr(char *fname,unsigned long fname_len)
{
	int m1,n1,l1;
	int fd = -1;
	Nbvars=0;
	CheckRhs(0,1);
	CheckLhs(1,1);
	/*  checking variable file */
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd = *istk(l1);
	}
	C2F(mclearerr)(&fd);
	LhsVar(1)= 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : merror */
/*--------------------------------------------------------------------------*/
int intsmerror(char *fname,unsigned long fname_len)
{
	int m1,n1,l1,un=1,lr;
	int fd=-1;
	Nbvars=0;
	CheckRhs(0,1);
	CheckLhs(1,1);
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd  = *istk(l1);
	}
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr);
	C2F(merror)(&fd,stk(lr));
	LhsVar(1)= Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
