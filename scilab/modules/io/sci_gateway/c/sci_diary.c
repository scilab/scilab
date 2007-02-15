/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "gw_io.h"
#include "sci_diary.h"
#include "../../fileio/src/files.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intdiary)();
/*-----------------------------------------------------------------------------------*/
int C2F(sci_diary) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intdiary)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int getdiary(void)
{
	return C2F(iop).wio;
} 
/*-----------------------------------------------------------------------------------*/
void C2F(diary)(char *str,int *n)
{
#if defined(_MSC_VER)
	char newline[2]="\r\n";
	int nn=2;
#else
	char newline[1]="\n";
	int nn=1;
#endif
	int u;

	u=getdiary();
	if ( u != 0 ) {
		FILE *fd= GetFile(&u);
		fwrite(str,sizeof(unsigned char),*n,fd);
		fwrite(newline,sizeof(unsigned char),nn,fd);
	}
}
/*-----------------------------------------------------------------------------------*/
void diary_nnl(char *str,int *n)
{
	int u;

	u=getdiary();
	if (u) {
		FILE *fd= GetFile(&u);
		fwrite(str,sizeof(unsigned char),*n,fd);
	}
}
/*-----------------------------------------------------------------------------------*/
