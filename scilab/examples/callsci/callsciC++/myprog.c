#include <stdio.h> 
#include "machine.h"
#include "stack-c.h"

#if (defined __MSC__ ) || (defined __MINGW32__) 
#define putenv(x) _putenv(x)
#endif

#ifdef __ABSC__
#define putenv(x) abs_putenv(x)
#endif

/* ---------------------------------------------
 * Initialize scilab 
 * If SCI is not specified in the Makefile 
 * the executable will only run if run from this 
 * directory (since SCI will be bound to ../../../) 
 * except if environment variables are set 
 * ---------------------------------------------*/

#ifndef SCI
#define SCI "SCI=../../.." 
#endif  

extern void add_sci_argv(char *p);

/* this function should be moved inside scilab */

static void Initialize()  
{
  char *p1;
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static iflag=-1, stacksize = 1000000, ierr=0;

  if ((p1 = getenv ("SCI")) == (char *) 0)
    {
#if (defined __MSC__ ) || (defined __MINGW32__) 
      set_sci_env(SCI,NULL);
#endif 
    }
  else
    {
#if (defined __MSC__ ) || (defined __MINGW32__) 
      set_sci_env(p1,NULL);
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

int send_scilab_job(char *job) 
{
  static char buf[1024];
  static char format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  fprintf(stderr,"job envoye %s\n",buf);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return (int) *stk(lp);
}

double f(double x)
{
  return x*x + 1;
}

static int my_plot() 
{
  int m,n,lx,ly,i;
  /* j'utilise scilab pour creer les abscisses 
   * et reserver de la place pour les ordonnees 
   */ 
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
  send_scilab_job("plot(y,x);xclick();quit");
}

static int my_job()
{
  static char job[256];
  /* storing a matrix inside scilab */
  static double A[]={1,2,3,4};
  int mA=2,nA=2;
  static double b[]={4,5};
  int mb=2,nb=1;
  int m,n,lp,i;
  WriteMatrix("A", &mA, &nA, A); // provoque un return 0 en cas d'echec 
  WriteMatrix("b", &mb, &nb, b);

  if ( send_scilab_job("A,b,x=0\\b;") != 0) 
    fprintf(stdout,"Error occured during scilab execution\n");
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

/* I do not want to see the Scilab banier */ 

void C2F(banier)(int *x) 
{
  /* fprintf(stdout,"Ourf ....\n");
     C2F(storeversion)("scilab-2.5.1",12L);
  */
}


int MAIN__(void) 
{
#if (defined __MSC__ ) || (defined __MINGW32__) 
  static char nw[]="-nw";
  static char nb[]="-nb";
  add_sci_argv(nw);
  add_sci_argv(nb);
#endif 
  Initialize();
  my_ode_job();
  my_job();
  my_plot() ;
  cc_test();
  C2F(sciquit)();
}

/*----------------------------------------------
 * Et la faisons des trucs vraiment tordus 
 *----------------------------------------------*/

/* je veux integrer l'equation qui precede */

int my_ode_job() 
{
  double x[]={1,0,0} ; int mx=3,nx=1;
  double time[]={0.4,4}; int mt=1,nt=2;
  fprintf(stdout,"je linke \n");
  send_scilab_job("ilib_for_link(''odeex'',''my_ode.o'',[],''c'');");
  fprintf(stdout,"fin du link  \n");
  send_scilab_job("exec(''loader.sce'');link(''show'')");
  WriteMatrix("x", &mx, &nx, x);
  WriteMatrix("time", &mt, &nt,time);
  /* pour que scilab <<linke>> mon_edo */
  /* appel de ode */
  send_scilab_job("y=ode(x,0,time,''mon_ode''),");
}






