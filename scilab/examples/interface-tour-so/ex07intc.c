#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 * Shows how to pass a Scilab list as argument 
 * and return a list (same for tlist and mlist)
 * 
 * GetRhsVar(n,'l',&m,&n,&l) is used to check that 
 * argument n is a list and m can be used to check the length 
 * of the list 
 * Then the arguments of the list can be accessed through 
 * GetListRhsVar and GetListRhsCVar with a limitation: 
 * "l" cannot be used as second argument of GetListRhsVar 
 * Note: also that PutLhsVar is not able to perform back 
 * conversion for list arguments. Thus, if a conversion was performed 
 * in the GetListRhsVar process (which is the case with 'i','c','r','z'
 * arguments) the list argument must not be returned. 
 * For example: 
 *    GetRhsVar(1,'l',...)
 *    GetListRhsVar(1,1,'c',...) 
 *    LhsVar(1)=1 
 * will return wrong data 
 * 
 * CreateVarFrom or CreateListVarFrom mus be used to provide 
 * correct code (see below)
 * 
 **************************************************/


/*--------------------------------------------------------
 * a list argument 
 *--------------------------------------------------------*/

int intex7c_1(fname)
  char* fname;
{ 
  char **Str2;
  int m=2,n=3,r1,c1,r2,r3,r4;
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,m6,n6,m7,n7;
  int it1,lc1;
  int i=0;
  int l;
  SciSparse S;
  SciIntMat Im;

  CheckRhs(1,1);
  CheckLhs(1,7);

  GetRhsVar(1,"l",&m,&n,&l);
  CheckLength(1,m,7);
  GetListRhsCVar(1,1,"d",&it1,&m1,&n1,&l1,&lc1);
  GetListRhsVar(1,2,"i",&m2,&n2,&l2);
  GetListRhsVar(1,3,"c",&m3,&n3,&l3);
  GetListRhsVar(1,4,"b",&m4,&n4,&l4);
  GetListRhsVar(1,5,"S",&m5,&n5,&Str2);
  GetListRhsVar(1,6,"s",&m6,&n6,&S);
  GetListRhsVar(1,7,"I",&m7,&n7,&Im);

  /* first list argument cannot be returned directly 
   * since the previous GetListRhsVar have performed 
   * data conversion. But we can create new return 
   * values from list argument with CreateVarFrom or 
   * CreateVarFromPtr.
   */ 

  CreateCVarFrom(2,"d",&it1,&m1,&n1,&r1,&c1,&l1,&lc1);
  CreateVarFrom(3,"i",&m2,&n2,&r2,&l2);
  CreateVarFrom(4,"c",&m3,&n3,&r3,&l3);
  CreateVarFrom(5,"b",&m4,&n4,&r4,&l4);

  CreateVarFromPtr( 6, "S", &m5, &n5, Str2);
  CreateVarFromPtr( 7, "s", &m6, &n6, &S);
  CreateVarFromPtr( 8, "I", &m7, &n7, &Im);

  for (i=1; i<= 7 ; i++) LhsVar(i) = i+1;

  /* we must free Str2 memory */ 
  FreeRhsSVar(Str2);

  return(0);
}


/*--------------------------------------------------------
 * here we create a return argument of type list 
 * CreateListVarFrom 
 *--------------------------------------------------------*/


int intex7c_2(fname) 
     char *fname;
{
  static int l1, m1, n1, l2, m2, n2, m3, n3, l3,m4,n4,l4,m5,n5,l5;
  static int m6=5,n6=1, l6,l61,l62,l63,l64,l65;

  SciIntMat Im;
  CheckRhs(5,5);
  CheckLhs(0,1);

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "i", &m2, &n2, &l2);
  GetRhsVar(3, "c", &m3, &n3, &l3);
  GetRhsVar(4, "b", &m4, &n4, &l4);
  GetRhsVar(5, "I", &m5, &n5, &Im);

  /* create a List with 5 elements  */
  CreateVar(6, "l", &m6, &n6, &l6);
  /* first element is filled with stk(l1) 
   * second with istk(l2) and third with cstk(l3). 
   * when element list are created 
   * they are backconverted to Scilab data types 
   * i.e stk(l61), stk(l62), istk(l63) can be used 
   * to acces data but istk(l62) and cstk(l63) no 
   */
  CreateListVarFrom(6,1,"d", &m1, &n1,&l61, &l1);
  CreateListVarFrom(6,2,"i", &m2, &n2,&l62, &l2);
  CreateListVarFrom(6,3,"c", &m3, &n3,&l63, &l3);
  CreateListVarFrom(6,4,"b", &m4, &n4,&l64, &l4);
  l65= Im.it; l5 = Im.l;
  CreateListVarFrom(6,5,"I", &m5, &n5,&l65, &l5);
  /*  Return variables  */
  LhsVar(1) = 6;
  return 0;
}


/*--------------------------------------------------------
 * here we create a return argument of type list (tlist,mlist)
 * and arguments with  CreateListVarFromPtr
 *--------------------------------------------------------*/

int  intex7c_3(fname) 
     char *fname;
{
  int m0,n0,l0;
  static double x1[]={10.0,20.0,30.0};int m1=1, n1=3; double *y1 = x1;
  static int    x2[]={1,2,3,4};       int m2=1, n2=4; int    *y2 = x2;
  static char   x3[]="a string";      int m3=strlen(x3),n3=1; char *y3=x3;
  static int    x4[]={1,0};           int m4=1, n4=2; int *y4 = x4;
  static char *Str[]= { "type","x1","x2", "x3","x4","x5"}; int m5=1,n5=6;
  int mL=7,nL=1,lL;
  /* a sparse matrix */ 
  static int mnel[5] ={0,2,3,1,1};
  static int icol[7] ={1,2,3,1,2,3,5};
  static double R[7] ={1.0,2.0,3.0,4.0,5.0,6.0,7.0};
  static SciSparse S ={ 5,5,0,7, mnel,icol,R,NULL};
  /* an int matrix of type uchar */ 
  static unsigned char V[] = { 41,52,63};
  static SciIntMat M ={ 1,3, 1, -1, V };
  
  CheckRhs(1,1);
  CheckLhs(0,1);
  GetRhsVar(1, "c", &m0, &n0, &l0);
  CheckLength(1,m0,1); 
  if ( *cstk(l0) != 'l' && *cstk(l0) != 't' &&  *cstk(l0) != 'm' ) *cstk(l0)='l';
    
  /* create a list (or tlist or mlist) with 4 elements  */
  CreateVar(1, cstk(l0), &mL, &nL, &lL);
  CreateListVarFromPtr(1,1,"S", &m5, &n5, Str);
  CreateListVarFromPtr(1,2,"d", &m1, &n1, &y1);
  CreateListVarFromPtr(1,3,"i", &m2, &n2, &y2);
  CreateListVarFromPtr(1,4,"c", &m3, &n3, &y3);
  CreateListVarFromPtr(1,5,"b", &m4, &n4, &y4);
  CreateListVarFromPtr(1,6,"s", &S.m, &S.n, &S);
  CreateListVarFromPtr(1,7,"I", &M.m, &M.n, &M);
  /*  Return variables  */
  LhsVar(1) = 1;
  return 0;
}





