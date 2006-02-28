#include <stdio.h> 
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
#if WIN32
	#define putenv(x) _putenv(x)
#endif
/*-----------------------------------------------------------------------------------*/ 
/* ---------------------------------------------
 * Initialize scilab 
 * If SCI is not specified in the Makefile 
 * the executable will only run if run from this 
 * directory (since SCI will be bound to ../../../) 
 * except if environment variables are set 
 * ---------------------------------------------*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef SCI
	#define SCI "SCI=../../.." 
#endif  
/*-----------------------------------------------------------------------------------*/ 
extern void add_sci_argv(char *p);
/*-----------------------------------------------------------------------------------*/ 
/* this function should be moved inside scilab */
static void Initialize()  
{
  char *p1;
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static iflag=-1, stacksize = 1000000, ierr=0;

  if ((p1 = getenv ("SCI")) == (char *) 0)
    {
#if (defined __MSC__ ) || (defined __MINGW32__) 
      set_sci_env(SCI);
#endif 
    }
  else
    {
#if (defined __MSC__ ) || (defined __MINGW32__) 
      set_sci_env(p1);
#endif 
    }
  
  /* Scilab Initialization */ 
  C2F(settmpdir)();
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }
  /* running the startup */ 
  C2F(scirun)(initstr,strlen(initstr));
  return;
}
/*-----------------------------------------------------------------------------------*/ 
int send_scilab_job(char *job) 
{
  static char buf[1024];
  static char format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return (int) *stk(lp);
}
/*-----------------------------------------------------------------------------------*/ 
double f(double x)
{
  return x*x + 1;
}
/*-----------------------------------------------------------------------------------*/ 
static int my_plot() 
{
  int m,n,lx,ly,i;
  /* Scilab is called to build the abscissae vector and 
     to allocate the ordinates */

  send_scilab_job("x=1:0.1:10;y=x;");
  GetMatrixptr("x", &m, &n, &lx);  
  GetMatrixptr("y", &m, &n, &ly);  
  /* 
   * y(i)=f(x(i))
   */
  for ( i=0; i < m*n ; i++) 
    *stk(ly+i) = f(*stk(lx+i));
  /* 
   * plot(x,y);
   */
  send_scilab_job("plot(y,x);");
}
/*-----------------------------------------------------------------------------------*/ 
static int my_job()
{
  static char job[256];
  /* storing a matrix inside scilab */
  static double A[]={1,2,3,4};
  int mA=2,nA=2;
  static double b[]={4,5};
  int mb=2,nb=1;
  int m,n,lp,i;
  WriteMatrix("A", &mA, &nA, A); /* if failed make my_job return O; */
  WriteMatrix("b", &mb, &nb, b);

  if ( send_scilab_job("A,b,x=0\\b;") == 27) 
    fprintf(stdout,"division by zero...\n");
  else 
    {
      GetMatrixptr("x", &m, &n, &lp);
      for ( i=0 ; i < m*n ; i++) 
	fprintf(stdout,"x[%d] = %5.2f\n",i,*stk(i+lp));
    }

  if ( send_scilab_job("A,b,x=A\\b;") != 0) 
    {
      fprintf(stdout,"Error occured during scilab execution\n");
    }
  else 
    {
      GetMatrixptr("x", &m, &n, &lp);
      for ( i=0 ; i < m*n ; i++) 
	fprintf(stdout,"x[%d] = %5.2f\n",i,*stk(i+lp));
    }
} 
/*-----------------------------------------------------------------------------------*/ 
/* I want to integrate the previous equation */
int my_ode_job() 
{
  double x[]={1,0,0} ; int mx=3,nx=1;
  double time[]={0.4,4}; int mt=1,nt=2;
  fprintf(stdout,"linking \n");
  send_scilab_job("ilib_for_link(''odeex'',''my_ode.o'',[],''c'');");
  fprintf(stdout,"link done  \n");
  send_scilab_job("exec(''loader.sce'');link(''show'')");
  WriteMatrix("x", &mx, &nx, x);
  WriteMatrix("time", &mt, &nt,time);
  /* scilab is called to solve the ODE */
  send_scilab_job("y=ode(x,0,time,''mon_ode''),");
}
/*-----------------------------------------------------------------------------------*/ 
#if WIN32
	int main(void) 	
#else
	int MAIN__(void) 
#endif
{
  Initialize();
  my_ode_job();
  my_job();
  my_plot() ;
  cc_test();
  C2F(sciquit)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
