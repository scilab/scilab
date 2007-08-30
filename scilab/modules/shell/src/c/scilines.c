/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "scilines.h"
#include "core_math.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/
#define DEFAULT_NUMBERS_LINES 28
#define DEFAULT_NUMBERS_COLUMNS 72
#define MIN_NUMBERS_LINES 0
#define MIN_NUMBERS_COLUMNS 10
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

	C2F(iop).lct[4] = Max(MIN_NUMBERS_COLUMNS,colums);
	bOK = TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL setLinesSize(int lines)
{
	BOOL bOK = FALSE;

	C2F(iop).lct[1] = Max(MIN_NUMBERS_LINES,lines);
	bOK = TRUE;

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
