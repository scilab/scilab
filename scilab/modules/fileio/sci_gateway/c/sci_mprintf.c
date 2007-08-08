/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "Scierror.h"
#include "machine.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "gw_fileio.h"
/*-----------------------------------------------------------------------------------*/ 
int int_objprintf __PARAMS((char *fname,unsigned long fname_len))
{
  static int l1, m1, n1, lcount, rval, k, mx, mk, nk;
  char *ptrFormat=NULL;
  int i=0;
  int NumberPercent=0;

  Nbvars = 0;
  CheckRhs(1,1000);
  CheckLhs(0,1);
  if ( Rhs < 1 ) 
  { 
     Scierror(999,"Error:\tRhs must be > 0\r\n");
     return 0;
  }

  for (k=2;k<=Rhs;k++) 
  {
    if (VarType(k) !=1 && VarType(k) !=10) {OverLoad(k);return 0;}
  }

  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  ptrFormat=cstk(l1);
  for(i=0;i<(int)strlen(ptrFormat);i++)
  {
    if (ptrFormat[i]=='%') 
	{
      NumberPercent++;
      if (ptrFormat[i+1]=='%') {NumberPercent--;i++;}
    }
  }

  if (NumberPercent<Rhs-1)
  {
	Scierror(999,"mfprintf: Invalid format.\r\n");
	return 0;
  }

  mx=0;

  if (Rhs>=2) 
  {
    GetMatrixdims(2,&mx,&nk);
    for (k=3;k<=Rhs;k++) 
	{
      GetMatrixdims(k,&mk,&nk);
      mx = Min(mx,mk);
    }
  }
  lcount = 1;
  if (Rhs == 1) rval=do_xxprintf("printf",stdout,cstk(l1),Rhs,1,lcount,(char **)0);
  else while (1) 
  {
	if ((rval = do_xxprintf("printf",stdout,cstk(l1),Rhs,1,lcount,(char **)0)) < 0) break;
	lcount++;
	if (lcount>mx) break;
  }
  if (rval == RET_BUG) return 0;
  LhsVar(1)=0; /** No return value **/
  PutLhsVar();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
