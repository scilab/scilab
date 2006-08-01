/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_readxbm.h"
#include "MALLOC.h"
#include "../src/gd/gd.h"
extern void C2F(readxbmimg)(char * string,gdImagePtr *imgptr,int *m,int *n,int *ncol);
extern void C2F(getgifpixels)(gdImagePtr *imgptr,unsigned char *pixels);
extern void C2F(deallocategifimg)(gdImagePtr *im);
extern void C2F(getgifcmap)(gdImagePtr *imgptr,unsigned char *cmap);
/*-----------------------------------------------------------------------------------*/ 
int sci_readxbm _PARAMS((char *fname,unsigned long fname_len))
{
	integer m1,n1,l1;

	//CheckLhs(2,2);
	//CheckRhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *FilenameXBM=NULL;
		gdImagePtr imgptr;
		int m,n,ncol;
		int ml1,nl1;
		int ml2,nl2;

		int* ArrayTmpL1=NULL;
		int* ArrayTmpL2=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		FilenameXBM=cstk(l1);

		C2F(readxbmimg)(FilenameXBM,&imgptr,&m,&n,&ncol);
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

		{
			int i=0;
			for(i=0;i<ml1*nl1;i++) ArrayTmpL1[i]=9;
			for(i=0;i<ml2*nl2;i++) ArrayTmpL2[i]=9;
		}


	/*	{
			int i,j,b=0;
			for (i=0;i<ml1;i++)
			{
				for (j=0;j<nl1;j++)
				{
					ArrayTmpL1[b] = imgptr->pixels[j][i];
					sciprint("%d ",ArrayTmpL1[b]);
				}
				sciprint("\n");
			}
			sciprint("\n");
		}

		{
			int i,b=0;

			int k = 0;
			for (i=0; (i<ncol); i++) 
			{
				ArrayTmpL2[k]        = imgptr->red[i];
				ArrayTmpL2[ncol+k]   = imgptr->green[i];
				ArrayTmpL2[2*ncol+k] = imgptr->blue[i];
				k++;
			}
		}
		
	*/	
//		CreateVarFromPtr(Rhs+2, "i",&ml1,&nl1,&ArrayTmpL1);
		//LhsVar(2)=Rhs+2;

		CreateVarFromPtr(Rhs+1, "i",&ml2,&nl2,&ArrayTmpL2);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		if (ArrayTmpL2){FREE(ArrayTmpL2);ArrayTmpL2=NULL;}
		if (ArrayTmpL1){FREE(ArrayTmpL1);ArrayTmpL1=NULL;}
		C2F(deallocategifimg)(&imgptr);
		
	}
	else
	{
		Scierror(999,"Incorrect xbm file\n");
		return 0;
	}
	
	
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
