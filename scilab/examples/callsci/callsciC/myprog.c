/*------------------------------------------------------------*/
/* Modified by Allan CORNET INRIA Novembre 2004 */
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
 * pour ne pas avoir a ecrire un script 
  */
/*------------------------------------------------------------*/
#ifndef SCI 
#define SCI "../.."
#endif 
/*------------------------------------------------------------*/
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void set_sci_env(char *p);
#ifdef WIN32
extern void add_sci_argv(char *p);
#endif
/*------------------------------------------------------------*/
#ifdef WIN32
static void SetEnv(void)
{
  #define MAXSTR 4096
  LPSTR tail;
  int i=0;
  char szModuleName[MAXSTR];
  char SCIPATH[MAXSTR];
  char WSCIPATH[MAXSTR];

  /* Get full name of this program */
  /* ex : c:\Program Files\scilab-3.0\bin\prog.exe */  
  GetModuleFileName ((HANDLE)GetModuleHandle(NULL),  (LPSTR) szModuleName, MAXSTR);
  
  /* remove prog.exe from szModuleName */ 	
  /* szModuleName --> c:\Program Files\scilab-3.0\bin\ */  	
  if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
  {
	tail++;
	*tail = '\0';
  }
  
  szModuleName[strlen(szModuleName)-1]='\0';
  /* copy szModuleName in WSCIPATH */
  wsprintf(WSCIPATH,"%s",szModuleName);
  /* remove bin in WSCIPATH */
  /* WSCIPATH --> c:\Program Files\scilab-3.0\ */  		
  if ((tail = strrchr (WSCIPATH, '\\')) != (LPSTR) NULL)
  {
	tail++;
	*tail = '\0';
  }
  
  /* copy WSCIPATH in SCIPATH */
  wsprintf(SCIPATH,"%s",WSCIPATH);
	
  /* convert \ on / in SCIPATH*/	
  for (i=0;i<(int)strlen(SCIPATH);i++)
  {
	if (SCIPATH[i]=='\\') SCIPATH[i]='/';
  }
	
  /* WSCIPATH --> c:\Program Files\scilab-3.0 */  			
  /* SCIPATH -->  c:/Program Files/scilab-3.0 */  			
  WSCIPATH[strlen(WSCIPATH)-1]='\0';
  SCIPATH[strlen(SCIPATH)-1]='\0';
	
  /* set scilab variables environment */
  set_sci_env(SCIPATH);
}
#endif
/*------------------------------------------------------------*/
static void Initialize() 
{

  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;
  /* set scilab variables environment */
   #ifdef WIN32
     SetEnv();
   #else
     setenv("SCI",SCI,1);
   #endif

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
/*------------------------------------------------------------*/
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
/*------------------------------------------------------------*/
static int premier_exemple()
{
  static double A[]={1,2,3,4};  int mA=2,nA=2;
  static double b[]={4,5};  int mb=2,nb=1;
  int m,n,lp,i;
  WriteMatrix("A", &mA, &nA, A);
  WriteMatrix("b", &mb, &nb, b);

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
  return 1;
} 
/*------------------------------------------------------------*/
static int deuxieme_exemple() 
{
  int m,n,lx,ly,i;
  /* j'utilise scilab pour creer les abscisses 
   * et reserver de la place pour les ordonnees 
   */ 
  send_scilab_job("x=1:0.1:10;y=x;");
  GetMatrixptr("x", &m, &n, &lx);  
  GetMatrixptr("y", &m, &n, &ly);  
  for ( i=0; i < m*n ; i++) 
    { 
      double xi = *stk(lx+i);
      *stk(ly+i) = xi*sin(xi);
    }
  /* plot(x,y);  */
  send_scilab_job("plot(x,y);xclick();quit");
  return 1;
}
/*------------------------------------------------------------*/
int troisieme_exemple() 
{
  double x[]={1,0,0} ; int mx=3,nx=1;
  double time[]={0.4,4}; int mt=1,nt=2;
  fprintf(stdout,"linking ... \n");
  #ifdef WIN32
  send_scilab_job("ilib_for_link(''odeex'',SCI+''/examples/callsci/callsciC/my_ode.o'',[],''c'');");
  #else
  send_scilab_job("ilib_for_link(''odeex'',''my_ode.o'',[],''c'');");
  #endif
  fprintf(stdout,"end of link  \n");
  send_scilab_job("link(''show'')");
  WriteMatrix("x", &mx, &nx, x);
  WriteMatrix("time", &mt, &nt,time);
  /* pour que scilab <<linke>> mon_edo */
  /* appel de ode */
  send_scilab_job("y=ode(x,0,time,''mon_ode''),");
  return 0;
}
/*------------------------------------------------------------*/
int MAIN__(void) 
{
#ifdef WIN32
  static char nw[]="-nw";
  static char nb[]="-nb";
  add_sci_argv(nw);
  add_sci_argv(nb);
#endif 	
  Initialize();
  
  premier_exemple();
  deuxieme_exemple() ;
  troisieme_exemple() ;
  
  C2F(sciquit)();
  return 0;
}
/*------------------------------------------------------------*/
