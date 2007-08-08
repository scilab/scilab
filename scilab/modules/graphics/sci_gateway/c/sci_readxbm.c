/*------------------------------------------------------------------------*/
/* file: sci_readxbm.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for sci_readxbm routine                               */
/*------------------------------------------------------------------------*/

#include "sci_readxbm.h"
#include "MALLOC.h"
#include "gifimg.h"
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/ 
int sci_readxbm ( char * fname,unsigned long fname_len )
{
	integer m1,n1,l1;

	CheckLhs(2,2);
	CheckRhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *FilenameXBM=NULL;
		gdImagePtr imgptr;
		int m,n,ncol;
		int ml1,nl1;
		int ml2,nl2;
		int i=0,j=0,k=0;
		int* ArrayTmpL1=NULL;
		int* ArrayTmpL2=NULL;
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		FilenameXBM=cstk(l1);

		readXbmImg(FilenameXBM,&imgptr,&m,&n,&ncol);
		ml1=m;
		nl1=n;

		ml2=ncol;
		nl2=3;

		if(m < 0)
		{
			Scierror(999,"Incorrect xbm file\n");
			return 0;
		}

		ArrayTmpL1=(int*)MALLOC(ml1*nl1*sizeof(int));
		ArrayTmpL2=(int*)MALLOC(ml2*nl2*sizeof(int));


		for (i=0;i<ml1;i++)
		{
			for (j=0;j<nl1;j++)
			{
				ArrayTmpL1[k] = imgptr->pixels[j][i];
				k++;
			}

		}
	
		k = 0;
		for (i=0; (i<ncol); i++) 
		{
			ArrayTmpL2[k]        = imgptr->red[i];
			ArrayTmpL2[ncol+k]   = imgptr->green[i];
			ArrayTmpL2[2*ncol+k] = imgptr->blue[i];
			k++;
		}
		
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&ml1,&nl1,&ArrayTmpL1);
		LhsVar(1)=Rhs+1;

		CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ml2,&nl2,&ArrayTmpL2);
		LhsVar(2)=Rhs+2;

		if ( ArrayTmpL2 != NULL ){FREE(ArrayTmpL2);ArrayTmpL2=NULL;}
		if ( ArrayTmpL1 != NULL ){FREE(ArrayTmpL1);ArrayTmpL1=NULL;}

		deallocateGifImg(&imgptr);
		
	}
	else
	{
		Scierror(999,"requires a xbm filename.\n");
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
