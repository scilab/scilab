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
#include "CallScilab.h"

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
/* See SCI/modules/core/includes/CallScilab.h */
/*------------------------------------------------------------*/
static int premier_exemple()
{
 static double A[]={1,2,3,4};  int mA=2,nA=2;
	static double b[]={4,5};  int mb=2,nb=1;


	/* Create Scilab matrices A and b */
	WriteMatrix("A", &mA, &nA, A);
	WriteMatrix("b", &mb, &nb, b);

	SendScilabJob("disp('A=');");
	SendScilabJob("disp(A);");
	SendScilabJob("disp('b=');");
	SendScilabJob("disp(b);");
	SendScilabJob("disp('x=A\\b');");

	if ( SendScilabJob("A,b,x=A\\b;") != 0) 
	{
		fprintf(stdout,"Error occured during scilab execution (SendScilabJob)\n");
	}
	else 
	{
		double *cxtmp=NULL;
		int m,n,lp,i;

		/* Get m and n */
		GetMatrixptr("x", &m, &n, &lp);

		cxtmp=(double*)malloc((m*n)*sizeof(double));

		ReadMatrix("x", &m, &n, cxtmp);

		for(i=0;i<m*n;i++)
		{
			fprintf(stdout,"x[%d] = %5.2f\n",i,cxtmp[i]);
		}

		if (cxtmp) 
		{
			free(cxtmp);
			cxtmp=NULL;
		}
	}
  return 0;
} 
/*------------------------------------------------------------*/
static int deuxieme_exemple() 
{
  SendScilabJob("plot3d();");
	printf("\nClose Graphical Windows to close this example.\n");
	while( ScilabHaveAGraph() )
	{
		ScilabDoOneEvent();
		Sleep(1);
	}
  return 1;
}
/*------------------------------------------------------------*/
int troisieme_exemple() 
{
  int code=0;

	char **JOBS=NULL;
	const int SizeJOBS=6;
	int i=0;

	JOBS=(char**)malloc(sizeof(char**)*SizeJOBS);

	for (i=0;i<SizeJOBS;i++)
	{
		JOBS[i]=(char*)malloc(sizeof(char*)*1024);
	}

	strcpy(JOBS[0],"A=1 ..");
	strcpy(JOBS[1],"+3;");
	strcpy(JOBS[2],"B = 8;");
	/* strcpy(JOBS[2],"b = V_NOT_EXIST;"); */
	strcpy(JOBS[3],"+3;");
	strcpy(JOBS[4],"disp('C=');");
	strcpy(JOBS[5],"C=A+B;disp(C);"); /* C = 12 */

	code=SendScilabJobs(JOBS,SizeJOBS);

	if (code)
	{
		char lastjob[4096]; // bsiz in scilab 4096 max
		if (GetLastJob(lastjob,4096))
		{
			printf("Error %s\n",lastjob);
		}
	}

	for (i=0;i<SizeJOBS;i++)
	{
		if (JOBS[i]) {free(JOBS[i]);JOBS[i]=NULL;}
	}
  return 0;
}
/*------------------------------------------------------------*/
#ifndef _MSC_VER
int MAIN__(void)
#else
int main(void)
#endif 
{
#ifdef WIN32
	if ( StartScilab(NULL,NULL,NULL) == FALSE ) printf("Error : StartScilab\n");
#else
    if ( StartScilab(SCI,NULL,NULL) == FALSE ) printf("Error : StartScilab\n");
 #endif

	printf("\nexample 1\n");  
	premier_exemple();
  
	printf("\nexample 2\n");  
	deuxieme_exemple() ;
	printf("\nexample 3\n");  
	troisieme_exemple() ;
	printf("\n\n");  
  
	if ( TerminateScilab(NULL) == FALSE ) printf("Error : TerminateScilab\n");
	return 0;
}
/*------------------------------------------------------------*/
