#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *     1- Accessing a Scilab Matrix (read and write) 
 *        by its name. 
 *****************************************/

int intex9c_1(fname)
     char* fname;
{ 
  int l1;
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  static int m, n, lp;
  int k;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /*
   * we want to acces scilab variable : param 
   * we can modify or read param with *stk(lp+k)
   */ 

  GetMatrixptr("param", &m, &n, &lp);
  CreateVar(1, "d",  &m, &n, &l1);

  for (k = 0; k < m*n ; ++k) 
    { 
      (*stk(l1+k)) = (*stk(lp+k));
    }
  
  LhsVar(1) = 1;
  return(0);
}


/*****************************************
 *  Accessing the Scilab Stack :
 *   Creation of a Scilab Matrix "C"
 *   from a name and an array of data
 *****************************************/

int intex9c_2(fname)
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


/*****************************************
 *  Accessing the Scilab Stack 
 *   Accessing a Scilab String (read) by its name. 
 *****************************************/

#define MAXCH 30

int intex9c_3(fname)
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


/*****************************************
 *  Accessing the Scilab Stack 
 *   Creation of a Scilab variable Str of type string
 *   from a name and an a C string 
 *****************************************/

int intex9c_4(fname)
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




