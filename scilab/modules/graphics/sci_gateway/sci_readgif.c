/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_readgif.h"
#include "MALLOC.h"
#include "../src/gd/gd.h"
extern void C2F(readgifimg)(char * string,gdImagePtr *imgptr,int *m,int *n,int *ncol);
extern void C2F(deallocategifimg)(gdImagePtr *im);
/*-----------------------------------------------------------------------------------*/ 
int sci_readgif _PARAMS((char *fname,unsigned long fname_len))
{
	integer m1,n1,l1;

	CheckLhs(2,2);
	CheckRhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *FilenameGIF=NULL;
		gdImagePtr imgptr;
		int m,n,ncol;
		int ml1,nl1;
		int ml2,nl2;
		int i=0,j=0,k=0;
		int* ArrayTmpL1=NULL;
		int* ArrayTmpL2=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		FilenameGIF=cstk(l1);

		C2F(readgifimg)(FilenameGIF,&imgptr,&m,&n,&ncol);
		ml1=m;
		nl1=n;

		ml2=ncol;
		nl2=3;

		if(m < 0)
		{
			Scierror(999,"Incorrect gif file\n");
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
		
		CreateVarFromPtr(Rhs+1, "i",&ml1,&nl1,&ArrayTmpL1);
		LhsVar(1)=Rhs+1;

		CreateVarFromPtr(Rhs+2, "i",&ml2,&nl2,&ArrayTmpL2);
		LhsVar(2)=Rhs+2;

		if (ArrayTmpL2){FREE(ArrayTmpL2);ArrayTmpL2=NULL;}
		if (ArrayTmpL1){FREE(ArrayTmpL1);ArrayTmpL1=NULL;}

		C2F(deallocategifimg)(&imgptr);
		
	}
	else
	{
		Scierror(999,"requires a gif filename.\n");
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
