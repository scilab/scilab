/* Copyright INRIA */

#include "C-LAB_Interf.h"
#include "gvar.h"

void LAB_setgvar()


{


  Matrix *Mcell;
  Matrix *Mname;

  Matrix *Mlist;
  int *m,i;
  int *name;

  
  

  Mname=Interf.Param[0];
  Mcell=Interf.Param[1];
  m=(int *)Mname;
  name = m+6;
  setGvar( name, Mcell);
/*    Mcell = MatrixCreateString("old");  */
/*      ReturnParam(Mcell);  */
  
/*   Mcell=Interf.Param[0]; */
/*   m = Mcell; */
/* for (i=0; i<6; i++) */
/*   printf(" %d \n", m[i]); */


	 
}
