#include "stack-c.h"

/* GetRhsVar with Lists */

typedef struct test {
  int x;
  char y[54];
} Test;

static Test myvar = { 10, "My string"};

/*----------------------------------------------
 * using a pointer to a C object at Scilab level 
 * intex10c_1 : returns a Scilab object used 
 *              to store a pointer 
 *----------------------------------------------*/

int intex10c_1(fname)
  char* fname;
{ 
  int m1=1,n1=1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVarFromPtr(1,"p",&m1,&n1,&myvar);
  /* other solution :
   * CreateVar(1,"p",&m1,&n1,&l1);
   * stk(l1) = ((unsigned long int) &myvar);
   */
  LhsVar(1) =1;
  return(0);
}

/*----------------------------------------------
 * intex10c_2 : getting a pointer from Scilab 
 *----------------------------------------------*/

int intex10c_2(fname)
  char* fname;
{
  static Test *myvar1;
  int m1=1,n1=1,l1;
  CheckRhs(1,1);
  CheckLhs(0,1);
  GetRhsVar(1,"p",&m1,&n1,&l1);
  myvar1 = (Test *) ((unsigned long int) *stk(l1));
  sciprint("Test structure : int = %d , y = %s\r\n",myvar1->x,myvar1->y);
  LhsVar(1) = 0;
  return(0);
}

/*----------------------------------------------
 * intex10c_3 : returns a Scilab tlist whith second 
 *              field stored witha pointer 
 *----------------------------------------------*/

int intex10c_3(fname)
  char* fname;
{ 
  static char *Str[]={"myh","h",NULL};
  int m11=1,n11=2,m12=1,n12=1,m1=2,n1=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* a tlist */
  CreateVar(1,"t", &m1, &n1, &l1);
  CreateListVarFromPtr(1,1,"S", &m11, &n11, Str);
  CreateListVarFromPtr(1,2,"p", &m12, &n12, &myvar);
  /*  Return variables  */
  LhsVar(1) = 1;
  return(0);
}

/*----------------------------------------------
 * intex10c_4 : get back the pointer from a Scilab tlist 
 *              created by intex10c_3
 *----------------------------------------------*/


int intex10c_4(fname)
  char* fname;
{
  static Test *myvar1;
  int m1,n1,l1,m12,n12,l12;
  CheckRhs(1,1);
  CheckLhs(0,1);
  GetRhsVar(1,"t",&m1,&n1,&l1);
  CheckLength(1,m1,2);
  GetListRhsVar(1,2,"p",&m12,&n12,&l12);
  myvar1 = (Test *) ((unsigned long int) *stk(l12));
  sciprint("Test structure : int = %d , y = %s\r\n",myvar1->x,myvar1->y);
  LhsVar(1) = 0;
  return(0);
}


