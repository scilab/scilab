#include <math.h>
#include <stdio.h> 
#include "machine.h"
#include "stack-c.h"

/* 
 * Scilab Initialization including Scilab startup execution
 * The Scilab path SCI has to be customized or passed by the makefile
 */

#ifndef SCI 
#define SCI "../.."
#endif 

#include <string.h> 

extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);

static void Initialize() 
{
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;

  setenv("SCI",SCI,1);
  /* Scilab Initialization */ 
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }
  /* running the startup */ 
  C2F(settmpdir)();

  C2F(scirun)(initstr,strlen(initstr));
}

int send_scilab_job(char *job) 
{
  static char buf[1024],
    format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return (int) *stk(lp);
}

/* first example */
static int premier_exemple()
{
  static double A[]={1,2,3,4};  int mA=2,nA=2;
  static double b[]={4,5};  int mb=2,nb=1;
  int m,n,lp,i;

  /* Create Scilab matrices A and b using C arrays A and b */
  WriteMatrix("A", &mA, &nA, A);
  WriteMatrix("b", &mb, &nb, b);

  if ( send_scilab_job("A,b,x=A\\b;") != 0) 
    {
      fprintf(stdout,"Error occured during scilab execution\n");
    }
  else 
    {
      /* get the "pointer" on the scilab matrix x 
	 the ith x entry can be adressed as *stk(i+lp) */
      GetMatrixptr("x", &m, &n, &lp); 
      for ( i=0 ; i < m*n ; i++) 
	fprintf(stdout,"x[%d] = %5.2f\n",i,*stk(i+lp));
    }
  return 1;
} 
/* second example */
static int deuxieme_exemple() 
{
  int m,n,lx,ly,i;
  /* Scilab is called to compute the abscissae and allocate memory for y */
  send_scilab_job("x=1:0.1:10;y=x;");
  /* get the "pointer" on the scilab matrix x and y */
  GetMatrixptr("x", &m, &n, &lx);  
  GetMatrixptr("y", &m, &n, &ly); 
  /* compute the y entry values */ 
  for ( i=0; i < m*n ; i++) 
    { 
      double xi = *stk(lx+i);
      *stk(ly+i) = xi*sin(xi);
    }
  /* call Scilab to do the plot */
  send_scilab_job("plot(x,y);xclick();quit");
  return 1;
}

/* third example */
int troisieme_exemple() 
{
  double x[]={1,0,0} ; int mx=3,nx=1;
  double time[]={0.4,4}; int mt=1,nt=2;
  fprintf(stdout,"Incremental linking \n");
  /* call scilab to link my_ode with itself */
  send_scilab_job("ilib_for_link(''odeex'',''my_ode.o'',[],''c'');");
  fprintf(stdout,"link done \n");
  send_scilab_job("link(''show'')");

 /* Create Scilab matrices x and time using C arrays x and time */
  WriteMatrix("x", &mx, &nx, x);
  WriteMatrix("time", &mt, &nt,time);
  /* call scilab to solve the ODE */
  send_scilab_job("y=ode(x,0,time,''mon_ode''),");
  return 0;
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
  Initialize(); /* initialize Scilab */
  premier_exemple(); /* execute the first example */
  deuxieme_exemple() ;/* execute the second example */
  troisieme_exemple() ; /* execute the third example */
  C2F(sciquit)(); /* terminate Scilab */
  return 0;
}




