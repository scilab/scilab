#include "intwintools.h"


int inttestwintools _PARAMS((char *fname, unsigned long fname_len))
{
	sciprint("\nIntroduce Wintools Interface\n");
	LhsVar(0)=0;
	C2F(putlhsvar)();
	return 0;
}


 static WintoolsTable Tab[]=
 {
  {inttestwintools,"testwintools"}
 };


/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/
int C2F(intwintools)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  return 0;
}
/*-----------------------------------------------------------------------------------*/
