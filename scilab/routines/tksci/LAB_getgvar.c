/* Copyright INRIA */

#include "C-LAB_Interf.h"
#include "gvar.h"

void LAB_getgvar()
{
  Matrix *Mcell;
  int *m, *name;
  /* jpc 2002 !!!!! name[0]=0; */
  m=(int *) Interf.Param[0];
  name = (int *) ( m )+6;
  getGvar((Matrix *) name, &Mcell);
  ReturnParam(Mcell);
  /*    Mcell = MatrixCreateString("old");  */
  /*      ReturnParam(Mcell);  */
  /*   Mcell=Interf.Param[0]; */
  /*   m = Mcell; */
  /* for (i=0; i<6; i++) */
  /*   printf(" %d \n", m[i]); */
}
