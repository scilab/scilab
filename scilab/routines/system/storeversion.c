#include <string.h>
#include "../stack-c.h"
#include "../version.h" 

static char Version[]=VERSION; 

int C2F(intversion)(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  static int n1=1,m1;
  char *v = Version ;
  Rhs=Max(0,Rhs);
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVarFromPtr( 1, "c",(m1=strlen(Version), &m1),&n1,&v);
  LhsVar(1) = 1;
  C2F(putlhsvar)();
  return(0);
}
