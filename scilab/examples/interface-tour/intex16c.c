#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *   Creation of a Scilab variable Str of type string
 *   from a name and an a C string 
 *****************************************/

int intex16c(fname)
     char* fname;
{ 
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  static char str[] ="Pilgrim said he, where can it be this land of Eldorado";
  int strl;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  strl=strlen(str);
  /* Creating the Scilab variable Str from str */
  WriteString("Str", &strl, str);

  /* No output */
  LhsVar(1) = 0;
  return(0);
}


