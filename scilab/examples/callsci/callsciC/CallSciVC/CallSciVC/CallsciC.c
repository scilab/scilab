/* Allan CORNET */
/* Scilab, INRIA 2004 */
/* Only For Windows */
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../../../../bin/libScilab.lib")
/*-----------------------------------------------------------------------------------*/

#include <windows.h>
#include <math.h>
#include <stdio.h> 
#include <string.h> 
#include <conio.h> 

#include "../../../../../routines/machine.h"
#include "../../../../../routines/stack-c.h"
#define TRUE 1
#define FALSE 0
/*-----------------------------------------------------------------------------------*/
/* See routines/system/CallScilab.h */
extern int StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);
extern int TerminateScilab(char *ScilabQuit);
extern int SendScilabJob(char *job); 

extern void ScilabDoOneEvent(void);
extern int ScilabHaveAGraph(void);
extern void C2F(tmpdirc)(void);

extern int SendScilabJobs(char **jobs,int numberjobs);
extern int GetLastJob(char *Job);
/*-----------------------------------------------------------------------------------*/
static int example1(void)
{

	static double A[]={1,2,3,4};  int mA=2,nA=2;
	static double b[]={4,5};  int mb=2,nb=1;


	/* Create Scilab matrices A and b */
	WriteMatrix("A", &mA, &nA, A);
	WriteMatrix("b", &mb, &nb, b);

	SendScilabJob("disp(''A='');");
	SendScilabJob("disp(A);");
	SendScilabJob("disp(''b='');");
	SendScilabJob("disp(b);");
	SendScilabJob("disp(''x=A\\b'');");

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
/*-----------------------------------------------------------------------------------*/
static int example2(void)
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
/*-----------------------------------------------------------------------------------*/
static int example3(void)
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
	strcpy(JOBS[5],"C=A+B;disp(C);");

	code=SendScilabJobs(JOBS,SizeJOBS);

	if (code)
	{
		char lastjob[4096]; // bsiz in scilab 4096 max
		if (GetLastJob(lastjob))
		{
			printf("Error %s\n",lastjob);
		}
	}

	for (i=0;i<SizeJOBS;i++)
	{
		if (JOBS[i]) {free(JOBS[i]);JOBS[i]=NULL;}
	}
	return 1;
}
/*-----------------------------------------------------------------------------------*/
int main(void)
/* int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow) */
{
	if ( StartScilab(NULL,NULL,NULL) == FALSE ) printf("Error : StartScilab \n");
	printf("\nexample 1\n");
	example1();
	system("pause");
	printf("\nexample 2\n");
	example2();
	system("pause");
	printf("\nexample 3\n");
	example3();
	system("pause");
	printf("\n\n");
	if ( TerminateScilab(NULL) == FALSE ) printf("Error : TerminateScilab \n");

	// Clean TMP DIRECTORY of scilab
	C2F(tmpdirc)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/

