#include "stack-c.h"


/**************************************************
 * examples of an hand written interface 
 * Shows how to pass string matrix as argument 
 *       how to return string arrays as Scilab string matrices
 **************************************************/

int intex5c_1(fname)
  char* fname;
{ 
  static char *Str[]= { "Un", "Deux","Trois", "Quatre","Cinq","Six" };
  char **Str2;
  int m=2,n=3;

  CheckRhs(1,1) ;
  CheckLhs(1,2) ;

  /* Checks that first argument is a Scilab String matrix 
   * returns the size in (m,n) and a NULL terminated array 
   * (columnwize)  of strings in Str2. 
   * Note that, since strings are stored in a special 
   * way inside Scilab stack, Str2 does not point to 
   * Scilab data but Str2 is a C object allocated on the execution stack. 
   * Str2 must be freed (See FreeRhsSVar below) 
   */ 

  GetRhsVar(1,"S",&m,&n,&Str2);

  /* now we can use Str2 to create a new Scilab string matrix */

  CreateVarFromPtr( 2, "S", &m, &n, Str2);

  /* but we can also create a string matrix from a C string array 
   * (which can be non-null terminated 
   */

  m=2;n=3;
  CreateVarFromPtr( 3, "S", &m, &n, Str);
  LhsVar(1) = 2;
  LhsVar(2) = 3;
  FreeRhsSVar(Str2);
  return(0);
}

/* search if first argument (a string) is 
 * an element of second argument (a string matrix) 
 */

int intex5c_2(fname)
  char* fname;
{ 
  char **Str2;
  int m,n,un=1,l1,i=0,l3;

  CheckRhs(2,2) ;
  CheckLhs(1,1) ;

  GetRhsVar(1,"c",&m,&n,&l1); /* a string */ 
  GetRhsVar(2,"S",&m,&n,&Str2); /* a string matrix */ 
  CreateVar(3,"d",&un,&un,&l3);
  
  i=0;
  while ( Str2[i] != NULL) 
    {
      if (strcmp(cstk(l1),Str2[i])==0) break;
      i++;
    }
  /* return the index */ 
  *stk(l3) = i+1;

  LhsVar(1) = 3;
  FreeRhsSVar(Str2);
  return(0);
}


