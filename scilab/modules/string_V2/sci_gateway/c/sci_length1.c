/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include <ctype.h> 
#include "returnProperty.h"

#include <stdio.h>

#include "machine.h"

#include "MALLOC.h" /* MALLOC */


/*-----------------------------------------------------------------------------------*/
//extern int C2F(intlength) _PARAMS((int *id));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_length1) _PARAMS((char *fname,unsigned long fname_len))
{
char typ = '*';

  char **Str;
  static char def_sep[] ="";
  char *sep = def_sep;
  static int un=1;
  char *text;
  int m1,n1,mn,i,j,k,m2,n2,l2,m3,n3,l3,nchars=0;
  int length=0;

  Rhs = Max(0, Rhs);

  CheckRhs(1,3);
  CheckLhs(1,1);
  
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    if (Rhs >= 2) {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      sep = cstk(l2);
    }
    if (Rhs >= 3) {
      GetRhsVar(3,"c",&m3,&n3,&l3);
      if ( m3*n3 != 0) 
		  typ = cstk(l3)[0];
      if (typ != 'c' && typ != 'r' && typ != 'p' && typ != 'a') {
		  Scierror(999,"%s: third argument should be 'c' or 'r'\r\n",fname);
		  return 0;
      }
    }
    switch ( typ ) {
    case '*' : 
      /* just return one string */ 
		for ( i = 0 ; i < mn ; i++ )
		  nchars += strlen(Str[i]);

      nchars += (mn-1)*strlen(sep);
      CreateVar(Rhs+1,"c",&un,&nchars,&l3);
      k=0;
      for ( i = 0 ; i < mn ; i++ ) 
	{
	  for ( j =0 ; j < (int)strlen(Str[i]) ; j++ ) 
	    *cstk(l3+ k++) = Str[i][j];
	  if ( i != mn-1) 
	    for ( j =0 ; j < (int)strlen(sep) ; j++ ) 
	      *cstk(l3+ k++) = sep[j];
	}
     // FreeRhsSVar(Str);
      LhsVar(1) = Rhs+1  ;
      break;
    

   
	case 'p':
		length=strlen(Str[0]);
		sciReturnInt(length);
		//FreeRhsSVar(Str);
		LhsVar(1) = Rhs+1  ;


    break;

	case 'a':
		text=*Str;
		sciReturnString(text);
		//FreeRhsSVar(Str);
		LhsVar(1) = Rhs+1  ;


  break;
  default : 
    OverLoad(1);
    break;
  }
  }
  
  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/
