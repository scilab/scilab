#pragma comment(lib, "../../../../../bin/libScilab.lib")


#ifndef SCI 
#define SCI "D:\\SCILAB"
#endif 

#include <windows.h>
#include <math.h>
#include <stdio.h> 
#include <string.h> 

#include "../../../../../routines/machine.h"
#include "../../../../../routines/stack-c.h"

extern void InitWindowGraphDll(void);
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void set_sci_env(char *p, char *wsci);

static void Initialize() 
{

  char *p1;
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static iflag=-1, stacksize = 1000000, ierr=0;

  if ((p1 = getenv ("SCI")) == (char *) 0)
    {
      set_sci_env(SCI,NULL);
    }
  else
    {
      set_sci_env(p1,NULL);
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
  C2F(scirun)(initstr,strlen(initstr));
}

void send_scilab_job(char *job) 
{
  static char buf[1024],
  format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  C2F(scirun)(buf,strlen(buf));
}


static int exemple()
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


void main(void)

{
  Initialize(); /* initialize Scilab */
  exemple(); 
  C2F(sciquit)(); /* terminate Scilab */
  return 0;
}




