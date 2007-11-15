/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                              */
/* Author : Allan Cornet                                                             */
/* desc : interface for prompt primitive                                             */
/*-----------------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "prompt.h"
/*-----------------------------------------------------------------------------------*/
#define PROMPT_SIZE_MAX 64
/*-----------------------------------------------------------------------------------*/
int C2F(sci_prompt) _PARAMS((char *fname,unsigned long fname_len))
{
	char currentPrompt[PROMPT_SIZE_MAX];
	static int n1 = 0, m1 = 0;
	int outIndex = 0 ;

	CheckRhs(0,0);
	CheckLhs(0,1);

	GetCurrentPrompt(currentPrompt);

	m1= (int)strlen(currentPrompt);
	n1=1;

	CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
	strcpy(cstk(outIndex), currentPrompt);

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
