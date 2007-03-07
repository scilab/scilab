#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack :
 *   Creation of a Scilab Matrix "C"
 *   from a name and an array of data
 *****************************************/
int intex14c(fname)
     char* fname;
{ 
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  static int m=1, n=3;
  static double C[3]={10,20,30};

  /*  zero input and one output (ans) of type 0 */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /** sending array C[] to Scilab as variable C (size [1,3]) **/
  WriteMatrix("C", &m, &n, C);
  /* LhsVar(1) = 0  means "no output" */
  LhsVar(1) = 0;
  return(0);
}
