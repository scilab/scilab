#include <stdio.h>
#include <stdlib.h>

#include "..\..\..\routines\machine.h"
#include "..\..\..\routines\stack-c.h"


extern void add_sci_argv(char *p);

/* this function should be moved inside scilab */
/************************************************************************/
static void Initialize()
{
  static char initstr[]="exec(\"..\\scilab.star\",-1);quit;";
  static iflag=-1, stacksize = 1000000, ierr=0;

  _control87(0x0008001F,0x0008001F); // Remove floating point exception

  /* Chemins à modifier suivant votre configuration */
  set_sci_env("..\\");

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
/************************************************************************/
int send_scilab_job(char *job)
{
  static char buf[1024];
  static char format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  fprintf(stderr,"job envoye %s\n",buf);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return 0;
  //return (int) *stk(lp);
}
/************************************************************************/
static int my_job()
{
   static double A[]={1.2,2.3,3.4,4.5};
   static double b[]={1.5,1.4,1.3,1.2};
   double x[4];
   int mA=2,nA=2;
   int mb=2,nb=2;
   int m,n,lp,i;
   // Creation de la matrice A dans Scilab
   WriteMatrix("A", &mA, &nA, A);
   // Creation de la matrice A dans Scilab
   WriteMatrix("b", &mb, &nb, b);
   // Affichage de la matrice b dans Scilab
   fprintf(stdout,"Affichage de la matrice b depuis Scilab\n");
   send_scilab_job("disp(b);");
   // Affichage de la matrice A dans Scilab
   fprintf(stdout,"Affichage de la matrice A depuis Scilab\n");
   send_scilab_job("disp(A);");
   // demande d'addition de A+b dans la matrice x dans scilab
   send_scilab_job("x=A+b;");
   // Affichage de la matrice x dans Scilab
   fprintf(stdout,"Affichage de la matrice x depuis Scilab\n");
   send_scilab_job("disp(x);");

   // Recuperation de la matrice dans le programme C/C++
   ReadMatrix("x", &m, &n, &x);
   // Affichage en C/C++ de la matrice x
   fprintf(stdout,"Affichage de la matrice x depuis le C/C++\n");
   for ( i=0 ; i < m*n ; i++)
   {
   	fprintf(stdout,"x[%d] = %5.2f\n",i,x[i]);
   }
}
/************************************************************************/
int MAIN__(void)
{

  static char nw[]="-nw";
  static char nb[]="-nb";
  add_sci_argv(nw);
  add_sci_argv(nb);

  Initialize();
  my_job();
  getch();
  C2F(sciquit)(); // quitte scilab

}


