/* Allan CORNET */
/* Scilab, INRIA 2004 */
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../../../../bin/libScilab.lib")
/*-----------------------------------------------------------------------------------*/
#ifndef SCI 
#define SCI "D:\\SCILAB"
#endif 

#include <windows.h>
#include <math.h>
#include <stdio.h> 
#include <string.h> 
#include <conio.h> 

#include "../../../../../routines/machine.h"
#include "../../../../../routines/stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern void InitWindowGraphDll(void);
extern int C2F(sxevents)();
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void set_sci_env(char *p);
extern void add_sci_argv(char *p);
/*-----------------------------------------------------------------------------------*/
static void Initialize() 
{
  char env[MAX_PATH + 1 + 10];
  char *p1;
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static iflag=-1, stacksize = 1000000, ierr=0;

  wsprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.4", SCI);
  putenv (env);

  wsprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.4", SCI);
  putenv (env);

  if ((p1 = getenv ("SCI")) == (char *) 0)
    {
      set_sci_env(SCI);
    }
  else
    {
      set_sci_env(p1);
    }
  
  /* Scilab Initialization */ 
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }

  C2F(settmpdir)();
  /* Initialisation fenetre graphique */
  InitWindowGraphDll();

  /* running the startup */ 
  C2F(scirun)(initstr,(int)strlen(initstr));
}
/*-----------------------------------------------------------------------------------*/
void send_scilab_job(char *job) 
{
  static char buf[1024],
  format[]="Err=execstr('%s','errcatch','n');quit;";
  sprintf(buf,format,job);
  C2F(scirun)(buf,(int)strlen(buf));
}
/*-----------------------------------------------------------------------------------*/
static int exemple1()
{
  static double A[]={1,2,3,4};  int mA=2,nA=2;
  static double b[]={4,5};  int mb=2,nb=1;
  int m,n,lp,i;

  /* Create Scilab matrices A and b */
  WriteMatrix("A", &mA, &nA, A);
  WriteMatrix("b", &mb, &nb, b);

  send_scilab_job("A,b,x=A\\b;");
  send_scilab_job("disp(A);");
  send_scilab_job("disp(b);");

  if ( ! C2F(cmatptr)("x", &m, &n, &lp,strlen("x"))) fprintf(stderr,"erreur lors de la reception \n");
  else 
  {
	  double *cxtmp;
	  cxtmp=(double*)malloc((m*n)*sizeof(double));
	  ReadMatrix("x", &m, &n, cxtmp);
	  for(i=0;i<m*n;i++)
	  {
		fprintf(stdout,"x[%d] = %5.2f\n",i,cxtmp[i]);
	  }
	  free(cxtmp);
  }
  
  return 1;
} 
/*-----------------------------------------------------------------------------------*/
static int exemple2()
{
	send_scilab_job("plot2d();");
	printf("\nPress a key to close this example.\n");
	while( !_kbhit() )
	{
		C2F (sxevents) ();
		Sleep(1);
	}
	return 1;
}
/*-----------------------------------------------------------------------------------*/
static int exemple3()
{
	send_scilab_job("scipad()");
	printf("\nPress a key to close this example.\n");
	while( !_kbhit() )
	{
		C2F (sxevents) ();
		Sleep(1);
	}
	return 1;
}
/*-----------------------------------------------------------------------------------*/
int MAIN__(void) 
{
#ifdef WIN32
  static char nw[]="-nw";
  static char nb[]="-nb";
  add_sci_argv(nw);
  add_sci_argv(nb);
#endif 	

  Initialize();

  exemple1();
  exemple2();

  /*exemple3();*/

  C2F(sciquit)();

  return 0;
}
/*-----------------------------------------------------------------------------------*/



