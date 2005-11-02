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
extern void C2F(tmpdirc)(void); 
/*------------------------------------------------------------*/
static int premier_exemple()
{
  static double A[]={1,2,3,4};  int mA=2,nA=2;
  static double b[]={4,5};  int mb=2,nb=1;
  int m,n,lp,i;
  
  SendScilabJob("disp(''A='');");
	SendScilabJob("disp(A);");
	SendScilabJob("disp(''b='');");
	SendScilabJob("disp(b);");
	SendScilabJob("disp(''x=A\\b'');");

  WriteMatrix("A", &mA, &nA, A);
  WriteMatrix("b", &mb, &nb, b);

  if ( SendScilabJob("A,b,x=A\\b;") != 0) 
    {
      fprintf(stdout,"Error occured during scilab execution\n");
    }
  else 
    {
      GetMatrixptr("x", &m, &n, &lp);
      for ( i=0 ; i < m*n ; i++) fprintf(stdout,"x[%d] = %5.2f\n",i,*stk(i+lp));
    }
  return 1;
} 
/*------------------------------------------------------------*/
static int deuxieme_exemple() 
{
  int m,n,lx,ly,i;
  /* j'utilise scilab pour creer les abscisses 
   * et reserver de la place pour les ordonnees 
   */ 
  SendScilabJob("x=1:0.1:10;y=x;");
  GetMatrixptr("x", &m, &n, &lx);  
  GetMatrixptr("y", &m, &n, &ly);  
  for ( i=0; i < m*n ; i++) 
    { 
      double xi = *stk(lx+i);
      *stk(ly+i) = xi*sin(xi);
    }
  SendScilabJob("disp(''Click on figure to continue'');");  
  /* plot(x,y);  */
  SendScilabJob("plot(x,y);xclick();quit");
  return 1;
}
/*------------------------------------------------------------*/
int troisieme_exemple() 
{
  double x[]={1,0,0} ; int mx=3,nx=1;
  double time[]={0.4,4}; int mt=1,nt=2;
  fprintf(stdout,"linking ... \n");
  #ifdef WIN32
  SendScilabJob("ilib_for_link(''odeex'',SCI+''/examples/callsci/callsciC/my_ode.o'',[],''c'');");
  #else
  SendScilabJob("ilib_for_link(''odeex'',''my_ode.o'',[],''c'');");
  #endif
  fprintf(stdout,"end of link  \n");
  SendScilabJob("link(''show'')");
  WriteMatrix("x", &mx, &nx, x);
  WriteMatrix("time", &mt, &nt,time);
  /* pour que scilab <<linke>> mon_edo */
  /* appel de ode */
  SendScilabJob("y=ode(x,0,time,''mon_ode''),");
  return 0;
}
/*------------------------------------------------------------*/
int MAIN__(void) 
{
#ifdef WIN32
	if ( StartScilab(NULL,NULL,NULL) == FALSE ) printf("Error : StartScilab \n");
#else
    if ( StartScilab(SCI,NULL,NULL) == FALSE ) printf("Error : StartScilab \n");
 #endif

	printf("\nexample 1\n");  
	premier_exemple();
  
	printf("\nexample 2\n");  
	deuxieme_exemple() ;
	printf("\nexample 3\n");  
	troisieme_exemple() ;
	printf("\n\n");  
  
	if ( TerminateScilab(NULL) == FALSE ) printf("Error : TerminateScilab \n");

	/* Clean TMPDIR of scilab */
	C2F(tmpdirc)();
	return 0;
}
/*------------------------------------------------------------*/
