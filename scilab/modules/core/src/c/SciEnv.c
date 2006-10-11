/*-----------------------------------------------------------------------------------*/
/* Sylvestre LEDRU */
/* Allan CORNET */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	extern void SciEnvForWindows(void);
#else
	extern int C2F(getsci)(char *buf,int *nbuf,long int lbuf);
#endif
/*-----------------------------------------------------------------------------------*/
/**
* Define SCI and some others Environments variables 
*/
void SciEnv(void) 
{
#ifdef _MSC_VER
	SciEnvForWindows(); 
#else
	#define PATH_MAX 1024

	char *buf;
	int  *nbuf;
	long int  lbuf;
	C2F(getsci)(buf,nbuf,PATH_MAX);
#endif
}
/*-----------------------------------------------------------------------------------*/
