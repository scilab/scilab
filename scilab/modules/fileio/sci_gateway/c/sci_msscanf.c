/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "machine.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "do_xxscanf.h"
#include "fileio.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int int_objsscanf(char *fname,unsigned long fname_len)
{
	static int l1, m1, n1,l2,m2,n2,iarg,maxrow,nrow,rowcount,ncol;
	int args,retval,retval_s,err,n_count,lw,il1,ild1,skip;
	int k;

	entry *data;
	rec_entry buf[MAXSCAN];
	sfdir  type[MAXSCAN],type_s[MAXSCAN];
	char* str;

	Nbvars = 0;
	CheckRhs(2,3);

	if (Rhs==3)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		if (m1*n1!=1)
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

	lw = iarg + Top - Rhs; /* Scilab string vector */
	if (! C2F(getwsmat)("sscanf",&Top,&lw,&m1,&n1,&il1,&ild1,6L)) return 0;
	GetRhsVar(iarg+1,STRING_DATATYPE,&m2,&n2,&l2); /* Format */
	n_count=StringConvert(cstk(l2))+1;  /* conversion */

	if ( (maxrow >= 0) && (maxrow*n_count>m1*n1) )
	{
		Scierror(999,_("%s: An error occurred: %s\n"),fname,_("Not enough entries."));
		return 0;
	}

	k=0;
	nrow=maxrow;
	rowcount = -1; /* number-1 of result lines already got */
	while (1)
	{
		rowcount++;
		if ((maxrow >= 0) && (rowcount >= maxrow)) break;
		if ( k >= m1*n1 ) break;
		skip=*istk(ild1+k)-1;
		SciStrtoStr(istk(il1+skip),&n_count,istk(ild1+k),&str);
		k +=n_count;

		args = Rhs; /* args set to Rhs on entry */
		err = do_xxscanf("sscanf",(FILE *)0,cstk(l2),&args,str,&retval,buf,type);
		FREE(str);
		if ( err < 0 )  return 0;

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

	/* create Scilab variables with each column of data */
	err=Sci_Store(rowcount,ncol,data,type_s,retval_s);
	Free_Scan(rowcount,ncol,type_s,&data);
	if (err==MEM_LACK) { Scierror(999,_("%s: No more memory.\n"),fname);}
	return 0;
}
/*--------------------------------------------------------------------------*/
