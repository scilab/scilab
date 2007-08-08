/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "machine.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "gw_fileio.h"
#include "fileio.h"
/*-----------------------------------------------------------------------------------*/ 
int int_objfprintfMat(char *fname,unsigned long fname_len)
{
	int l1, m1, n1,l2,m2,n2,m3,n3,l3,i,j,mS,nS;
	FILE  *f;
	char **Str2;
	char *Format;
	Nbvars = 0;
	CheckRhs(1,4); 
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);/* file name */
	}
	else
	{
		Scierror(999,"first parameter must be a filename.\n");
		return 0;
	}

	if (GetType(2) == sci_matrix)
	{
#define COMPLEXPART 1
		int *header=NULL;
		int Cmplx;

		header = (int *) GetData(2);
		Cmplx=header[3];

		if (Cmplx != COMPLEXPART)
		{
			GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); /* data */
		}
		else
		{
			Scierror(999,"%s works only with reals.\n",fname);
			return 0;
		}
	}
	else
	{
		Scierror(999,"%s works only with reals.\n",fname);
		return 0;
	}

	if ( Rhs >= 3) 
	{
		GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);/* format */
		StringConvert(cstk(l3));  /* conversion */
		Format = cstk(l3);
	}
	else 
	{
		Format = "%f";
	}

	if ( Rhs >= 4 )
	{
		GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&mS,&nS,&Str2);
	}

	if (( f = fopen(cstk(l1),"w")) == (FILE *)0) 
	{
		Scierror(999,"Error: in function %s, cannot open file %s\r\n",fname,cstk(l1));
		return 0;
	}

	if ( Rhs >= 4 )
	{
		for ( i=0 ; i < mS*nS ; i++) fprintf(f,"%s\n",Str2[i]);
	}

	for (i = 0 ; i < m2 ; i++ ) 
	{
		for ( j = 0 ; j < n2 ; j++) 
		{
			fprintf(f,Format,*stk(l2+i + m2*j));
			fprintf(f," ");
		}

		fprintf(f,"\n");
	}
	fclose(f);
	LhsVar(1)=0 ; /** no return value **/
	if ( Rhs >= 4) 
	{
		if (Str2)
		{
			for ( i=0 ; i < mS*nS ; i++) 
			{
				if (Str2[i])
				{
					FREE(Str2[i]);
					Str2[i]=NULL;
				}
			}
			FREE(Str2);
			Str2=NULL;
		}
	}
	PutLhsVar();
	return 0;
}  
/*-----------------------------------------------------------------------------------*/ 
