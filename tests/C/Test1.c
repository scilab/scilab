/*------------------------------------------------------------*/
/* Modified by Allan CORNET INRIA Mars 2005 */
/*------------------------------------------------------------*/
#include <math.h>
#include <stdio.h> 
#ifdef WIN32
  #include <windows.h> 
#endif

#include <string.h> 

#include "machine.h"
#include "stack-c.h"

/*------------------------------------------------------------*/
/* 
 * Initialisation de Scilab 
 * avec execution de la startup 
 */
/*------------------------------------------------------------*/
#ifndef SCI 
#define SCI "../.."
#endif 
/*------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0
/*------------------------------------------------------------*/
/* See routines/system/CallScilab.h */
extern int StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);
extern int TerminateScilab(char *ScilabQuit);
extern int SendScilabJob(char *job); 
/*------------------------------------------------------------*/
static int Test1(void)
{
  static double A[]={1,2,3,4};  int mA=2,nA=2;
  static double b[]={4,5};  int mb=2,nb=1;
  int m,n,lp,i;
  
  WriteMatrix("A", &mA, &nA, A);
  WriteMatrix("b", &mb, &nb, b);

  if ( SendScilabJob("A,b,x=A\\b;") != 0) 
    {
      fprintf(stdout,"Error occured during scilab execution\n");
    }
  else 
    {
      GetMatrixptr("x", &m, &n, &lp);
      for ( i=0 ; i < m*n ; i++) 
	fprintf(stdout,"x[%d] = %5.2f\n",i,*stk(i+lp));
    }
  return 1;
} 
/*------------------------------------------------------------*/
int MAIN__(void) 
{
	#ifdef WIN32
		if ( StartScilab(NULL,NULL,NULL) == FALSE ) 
		  printf("Error : StartScilab \n");
	#else
    if ( StartScilab(SCI,NULL,NULL) == FALSE ) 
      printf("Error : StartScilab \n");
  #endif
  
  Test1();
  
	if ( TerminateScilab(NULL) == FALSE ) 
		  printf("Error : TerminateScilab \n");
  return 0;
}
/*------------------------------------------------------------*/
