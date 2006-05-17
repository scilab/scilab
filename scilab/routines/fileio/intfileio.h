/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET (Reorganisation)*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTFILEIO__
#define __INTFILEIO__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

int intsmopen(char *fname,unsigned long fname_len);
int intsmputstr(char *fname,unsigned long fname_len);
int intsmclose(char *fname,unsigned long fname_len);
int intsmput(char *fname,unsigned long fname_len);
int intsmget(char *fname,unsigned long fname_len);
int intsmgetstr(char *fname,unsigned long fname_len);
int intsmeof(char *fname,unsigned long fname_len);
int intsmseek(char *fname,unsigned long fname_len);
int intsmtell(char *fname,unsigned long fname_len);
int intsmclearerr(char *fname,unsigned long fname_len);
int intsmerror(char *fname,unsigned long fname_len);

#endif /*  __INTFILEIO__ */
/*-----------------------------------------------------------------------------------*/
