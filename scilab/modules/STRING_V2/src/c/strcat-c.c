#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "machine.h"

#include "MALLOC.h" /* MALLOC */

#include "stack-c.h"
#include "Scierror.h"


/****************************************************
 * y= strcat(str1,sep,[op])
 * op = 'c' or 'r'
 ******************************************************/
int C2F(intstrcat) (char* fname);
/****************************************************/
int C2F(intstrcat) (char* fname)
{
  char typ = '*';
  char **Str,**Str1;
  static char def_sep[] ="";
  char *sep = def_sep;
  static int un=1;
  int m1,n1,mn,i,j,k,m2,n2,l2,m3,n3,l3,nchars=0;
  Rhs = Max(0, Rhs);

  CheckRhs(1,3);
  CheckLhs(1,1);

  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
    mn = m1*n1;
    if (Rhs >= 2) {
      GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
      sep = cstk(l2);
    }
    if (Rhs >= 3) {
      GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
      if ( m3*n3 != 0)
	typ = cstk(l3)[0];
      if (typ != 'c' && typ != 'r' ) {
	Scierror(999,"%s: third argument should be 'c' or 'r'\n",fname);
	return 0;
      }
    }
    switch ( typ )
	{
    case '*' :
      /* just return one string */
      for ( i = 0 ; i < mn ; i++ ) nchars += (int)strlen(Str[i]);
      nchars += (mn-1)*(int)strlen(sep);
      CreateVar(Rhs+1,STRING_DATATYPE,&un,&nchars,&l3);
      k=0;
      for ( i = 0 ; i < mn ; i++ )
	  {
		  for ( j =0 ; j < (int)strlen(Str[i]) ; j++ ) *cstk(l3+ k++) = Str[i][j];
		  if ( i != mn-1) for ( j =0 ; j < (int)strlen(sep) ; j++ ) *cstk(l3+ k++) = sep[j];
	  }
	  if (Str)
	  {
		  for ( i = 0 ; i < mn ; i++ )
		  {
			if (Str[i])
			{
				FREE(Str[i]);
				Str[i]=NULL;
			}
		  }
		  FREE(Str);
		  Str=NULL;
	  }
      LhsVar(1) = Rhs+1  ;
      break;
    case 'c':
      /* return a column matrix */
      if ( (Str1 = MALLOC((m1+1)*sizeof(char *)))==NULL)
	{
	  Scierror(999,"%s: Out of memory\n",fname);
	  return 0;
	}
      Str1[m1]=NULL;
      for (i= 0 ; i < m1 ; i++) {
	/* length of row i */
	nchars = 0;
	for ( j = 0 ; j < n1 ; j++ ) nchars += (int)strlen(Str[i+ m1*j]);
	nchars += (n1-1)*(int)strlen(sep);
	if ( (Str1[i]=MALLOC((nchars+1)*sizeof(char)))==NULL)
	  {
	    Scierror(999,"%s: Out of memory\n",fname);
	    return 0;
	  }
	/* fill the string */
	strcpy(Str1[i],Str[i]);
	for ( j = 1 ; j < n1 ; j++ )
	{
	  strcat(Str1[i],sep);
	  strcat(Str1[i],Str[i+ m1*j]);
	}
      }
      CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &un, Str1);
	  if (Str)
	  {
		  for ( i = 0 ; i < mn ; i++ )
		  {
			  if (Str[i])
			  {
				  FREE(Str[i]);
				  Str[i]=NULL;
			  }
		  }
		  FREE(Str);
		  Str=NULL;
	  }
      LhsVar(1) = Rhs+1  ;
	  if (Str1)
	  {
		  int i2=0;
		  for (i2=0;i2<m1+1;i2++)
		  {
			  if (Str1[i2])
			  {
				  FREE(Str1[i2]);
				  Str1[i2]=NULL;
			  }
		  }
		  FREE(Str1);
		  Str1=NULL;
	  }
      break;
    case 'r':
      /* return a row matrix */
      if ( (Str1 = MALLOC((n1+1)*sizeof(char *)))==NULL)
	{
	  Scierror(999,"%s: Out of memory\n",fname);
	  return 0;
	}
      Str1[n1]=NULL;
      for (j= 0 ; j < n1 ; j++) {
	/* length of col j */
	nchars = 0;
	for ( i = 0 ; i < m1 ; i++ )
	  nchars += (int)strlen(Str[i+ m1*j]);
	nchars += (m1-1)*(int)strlen(sep);
	if ( (Str1[j]=MALLOC((nchars+1)*sizeof(char)))==NULL)
	  {
	    Scierror(999,"%s: Out of memory\n",fname);
	    return 0;
	  }
	/* fill the string */
	strcpy(Str1[j],Str[j*m1]);
	for ( i = 1 ; i < m1 ; i++ ) {
	  strcat(Str1[j],sep);
	  strcat(Str1[j],Str[i+ m1*j]);
	}
      }
      CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &un, &n1, Str1);
	  if (Str)
	  {
		  for ( i = 0 ; i < mn ; i++ )
		  {
			  if (Str[i])
			  {
				  FREE(Str[i]);
				  Str[i]=NULL;
			  }
		  }
		  FREE(Str);
		  Str=NULL;
	  }
	  if (Str1)
	  {
		  int i2=0;
		  for (i2=0;i2<n1+1;i2++)
		  {
			  if (Str1[i2])
			  {
				  FREE(Str1[i2]);
				  Str1[i2]=NULL;
			  }
		  }
		  FREE(Str1);
		  Str1=NULL;
	  }
      LhsVar(1) = Rhs+1  ;
      break;
    }
    break;
  default :
    OverLoad(1);
    break;
  }

  C2F(putlhsvar)();
	return 0;
}
