/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "scilines.h"
#include "core_math.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/
#define DEFAULT_NUMBERS_LINES 72
#define DEFAULT_NUMBERS_COLUMNS 28
/*-----------------------------------------------------------------------------------*/
int scilines(int nl, int nc)
{
	setLinesSize(nl);
	setColumnsSize(nc);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int scilinesdefault(void)
{
	setLinesSize(DEFAULT_NUMBERS_LINES);
	setColumnsSize(DEFAULT_NUMBERS_COLUMNS);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(scilines)(int *nl, int *nc)
{
	return scilines(*nl,*nc);
}
/*-----------------------------------------------------------------------------------*/
int C2F(scilinesdefault)(void)
{
	return scilinesdefault();
}
/*-----------------------------------------------------------------------------------*/
BOOL setColumnsSize(int colums)
{
	BOOL bOK = FALSE;
	if (getColumnsSize() != 0) 
	{
		C2F(iop).lct[1] = Max(0,colums);
		bOK = TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL setLinesSize(int lines)
{
	BOOL bOK = FALSE;
	if (lines >= 0)
	{
		C2F(iop).lct[4] = lines;
		bOK = TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int getColumnsSize(void)
{
	return C2F(iop).lct[1];
}
/*-----------------------------------------------------------------------------------*/
int getLinesSize(void)
{
	return C2F(iop).lct[4];
}
/*-----------------------------------------------------------------------------------*/
