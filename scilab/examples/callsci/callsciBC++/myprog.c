#include <stdio.h>
#include <stdlib.h>

#include "..\..\..\routines\machine.h"
#include "..\..\..\routines\stack-c.h"

/* Allan CORNET */
/* INRIA */

#define TRUE 1
#define FALSE 0

/************************************************************************/
/* See routines/system/CallScilab.h */
extern int StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);
extern int TerminateScilab(char *ScilabQuit);
extern int SendScilabJob(char *job); 
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
   SendScilabJob("disp(b);");
   // Affichage de la matrice A dans Scilab
   fprintf(stdout,"Affichage de la matrice A depuis Scilab\n");
   SendScilabJob("disp(A);");
   // demande d'addition de A+b dans la matrice x dans scilab
   SendScilabJob("x=A+b;");
   // Affichage de la matrice x dans Scilab
   fprintf(stdout,"Affichage de la matrice x depuis Scilab\n");
   SendScilabJob("disp(x);");

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
int main(void)
{

	if ( StartScilab(NULL,NULL,NULL) == FALSE ) printf("Error : StartScilab \n");
  my_job();
  getch();
	if ( TerminateScilab(NULL) == FALSE ) printf("Error : TerminateScilab \n");

}
/************************************************************************/

