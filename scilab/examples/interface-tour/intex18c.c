#include "stack-c.h"

extern int dblearrayc __PARAMS((double **a, int *m, int *n,int *err));
extern int intarrayc __PARAMS((int **a, int *m, int *n,int *err));
extern int crestrc __PARAMS((char **a, int *m,int *err));

int intex18c(fname)
  char* fname;
{ 
  static char *Str[]= { "Un", "Deux","Trois", "Quatre","Cinq","Six" };
  char **Str2;
  int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;
  int m=2,n=3;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1,"S",&m,&n,&Str2);
  m=2;n=3;
  CreateVarFromPtr( 2, "S", &m, &n, Str);
  LhsVar(1) = 2;
  return(0);
}


