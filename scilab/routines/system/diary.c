#include <stdio.h>
#include "../machine.h"
#include "../stack-def.h"

extern FILE *GetFile(integer *fd);

int getdiary()
{
  return C2F(iop).wio;
} 
  
void C2F(diary)(char *str,int *n)
{
#if defined(MSDOS) || defined(_Windows) || defined(DOS386)
  char newline[2]="\r\n";
  int nn=2;
#else
  char newline[1]="\n";
  int nn=1;
#endif
  int u;

  u=getdiary();
  if ( u != 0 ) {
    FILE *fd= GetFile(&u);
    fwrite(str,sizeof(unsigned char),*n,fd);
    fwrite(newline,sizeof(unsigned char),nn,fd);
  }
}

void diary_nnl(char *str,int *n)
{
  int u;

  u=getdiary();
  if (u) {
    FILE *fd= GetFile(&u);
    fwrite(str,sizeof(unsigned char),*n,fd);
  }
}
