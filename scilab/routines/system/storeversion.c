#include "../machine.h"
#include "../graphics/Math.h" 
#include "../stack-c.h"
#include <string.h>
#define V_SIZE 50

static char Version[V_SIZE];
static int Version_size=0;

void C2F(storeversion)(str,n)
     char *str;
     int n;
{
  n = Min(V_SIZE,n);
  strncpy(Version,str,(size_t) n);
  Version[n]='\0';
  Version_size = n;
}

int C2F(intversion)(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  static int n1=1;
  char *v = Version ;
  Rhs=Max(0,Rhs);
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVarFromPtr( 1, "c", &Version_size,&n1,&v);
  LhsVar(1) = 1;
  C2F(putlhsvar)();
  return(0);
}
