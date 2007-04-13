/*------------------------------------------------------------------------*/
/* file: sci_xlfontt.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xlfont routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xlfont.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "Xcall1.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
int sci_xlfont( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,v,i,count,sz[10],num;
  double dv;

  SciWin();
  if (Rhs <= 0)
  {
    int one = 1 ;
    char **S;
    /*     we list the fonts and return their names in a string matrix */
    int m = 0;
    C2F(dr1)("xgfont",C2F(cha1).buf,&m,sz,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
    if (m == 0) { LhsVar(1)=0; return 0;}
    if (( S= (char **) MALLOC( (m+1)*sizeof(char*))) == NULL) 
    {
      Scierror(999,"%s: running out of memory \r\n",fname);
      return 0;
    }
    count =0;
    /* OS Windows: Pb here due to fonttab again: its size is 10 and NUMBERFONT is 11 (=m) so...*/
    /* By-pass here for the moment: FONTNUMBER set to FONTNUMBER-- in queryfamily (file periWin.c)*/
    for ( i = 0 ; i < m ; i++) {
      if ((S[i]= (char *) MALLOC((sz[i]+1)*sizeof(char))) == NULL) 
      {
        Scierror(999,"%s: running out of memory \r\n",fname);
        return 0;
      }
      strncpy(S[i],C2F(cha1).buf+count,sz[i]);
      count += sz[i]; 
      S[i][sz[i]]='\0';
    } 
    S[m]= (char *) 0;
    CreateVarFromPtr(Rhs+1,"S",&one,&m,S);
	if (S)
	{
		int j=0;
		for(j=0;j<m+1;j++)
		{
			if (S[j])
			{
				FREE(S[j]);
				S[j]=NULL;
			}
		}
		FREE(S);
		S=NULL;
	}
    LhsVar(1)=Rhs+1;
    return 0;
  }
  CheckRhs(2,2);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);  CheckScalar(2,m2,n2);  num = (integer) *stk(l2);
  C2F(dr1)("xlfont",cstk(l1),&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
