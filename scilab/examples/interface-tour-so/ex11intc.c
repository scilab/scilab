#include "stack-c.h"

/*
 * Scilab hypermat <-> C struct 
 * or how to match a list ( tlist/ mlist with a struct ) 
 */

typedef struct hypermat {
  SciIntMat sc; /* coding informations */
  int it,size;
  double *R, *I;
} HyperMat;


#define GetRhsHMat(pos,H) if (!get_rhs_hmat(pos,H)) { return 0;}

int get_rhs_hmat(int pos,HyperMat *H) 
{
  char **Str2;
  int m,n,l,m1,n1,m2,n2,m3,n3,l3,it3,lc3;
  SciIntMat l2;
  GetRhsVar(pos,"m",&m,&n,&l);
  CheckLength(pos,m,3);
  GetListRhsVar(pos,1,"S",&m1,&n1,&Str2);
  if ( strcmp(Str2[0],"hm") != 0) 
    {
      Scierror(999,"Argument %d is not an hypermatrix\r\n",pos);
      return 0;
    }
  GetListRhsVar(pos,2,"I",&m2,&n2,&H->sc);
  GetListRhsCVar(pos,3,"d",&it3,&m3,&n3,&l3,&lc3);
  H->size = m3*n3;
  H->R = stk(l3);
  H->I = (it3==1) ? stk(lc3): NULL;
  H->it= it3;
  /* we must free Str2 memory */ 
  FreeRhsSVar(Str2);
  return 1;
}


#define CreateHMat(pos,H) if (! cre_hmat(pos,H)) { return 0;}

int cre_hmat(int pos,  HyperMat *H)
{
  static char *Str[]= { "hm","dims","entries"}; int m1=1,n1=3;
  int mL=3,nL=1,lL, un=1;
  CreateVar(pos,"m", &mL, &nL, &lL);
  CreateListVarFromPtr(pos,1,"S", &m1, &n1, Str);
  CreateListVarFromPtr(pos,2,"I",&(H->sc.m), &(H->sc.n), &(H->sc));
  CreateListCVarFromPtr(pos,3,"d", &H->it,&H->size,&un ,&H->R,&H->I);
  return 1;
}


int intex11c(fname)
  char* fname;
{ 
  HyperMat M;
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsHMat(1,&M);
  CreateHMat(2,&M);
  LhsVar(1) =2;
  return(0);
}



     
