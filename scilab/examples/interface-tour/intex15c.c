#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *   Accessing a Scilab String (read) by its name. 
 *****************************************/

#define MAXCH 30

int intex15c(fname)
     char* fname;
{ 
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  char str[MAXCH];
  int strl=MAXCH;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* We search a Scilab Object named Mystr check that it is a string 
   * and store the string in str. 
   * strl is used on entry to give the maximum number 
   * of characters which can be stored in str 
   * After the call strl contains the number of 
   * copied characters
   */

  ReadString("Mystr", &strl, str);

  sciprint("Mystr= \"%s\", length %d \r\n", str, strl);

  /*    LhsVar(1) = 0; means "no output"   */
  LhsVar(1) = 0;
  return(0);
}

