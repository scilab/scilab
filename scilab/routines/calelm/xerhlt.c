#include <setjmp.h>
#include <string.h>
#include "../stack-c.h"

static  jmp_buf slatec_jmp_env; 

int C2F(setslatecjmp)()
{
  return setjmp(slatec_jmp_env);
}

void C2F(xerhlt) (char *messg, unsigned long l)
{
  longjmp(slatec_jmp_env,-1); 
}

