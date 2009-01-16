/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU (Detection of the term size)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
	#include <curses.h>
	#include <term.h>
#endif
#include "scilines.h"
#include "core_math.h"
#include "stack-def.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/console.h"
#include "scilabmode.h"
#endif
/*--------------------------------------------------------------------------*/
#define DEFAULT_NUMBERS_LINES 28
#define DEFAULT_NUMBERS_COLUMNS 80
#define MIN_NUMBERS_LINES 0
#define MIN_NUMBERS_COLUMNS 10
/*--------------------------------------------------------------------------*/
int scilines(int nblines, int nbcolumns)
{
	setLinesSize(nblines);
	setColumnsSize(nbcolumns);
	return 0;
}
/*--------------------------------------------------------------------------*/
int scilinesdefault(void)
{
#ifndef _MSC_VER

	char tc_buf[1024];       /* holds termcap buffer */
	if(tgetent(tc_buf, getenv("TERM")) == 1) {
		setLinesSize(tgetnum("li")); /* retrieve from the term info the number
										of lines */
		setColumnsSize(tgetnum("co")); /* And the number of columns */
	}else{
		/* Haven't been able to detect the terminal */
		setLinesSize(DEFAULT_NUMBERS_LINES);
		setColumnsSize(DEFAULT_NUMBERS_COLUMNS);
	}
	
#else
	if ( (getScilabMode() != SCILAB_STD) && (getScilabMode() != SCILAB_API) )
	{
		/* -NW or -NWNI mode */
		int X = getXConsoleScreenSize();
		int Y = getYConsoleScreenSize();

		if (X < DEFAULT_NUMBERS_COLUMNS) X = DEFAULT_NUMBERS_COLUMNS;
		if (Y < DEFAULT_NUMBERS_LINES) Y = DEFAULT_NUMBERS_LINES;
		setColumnsSize(X);
		setLinesSize(Y);
	}
	else
	{
		setLinesSize(DEFAULT_NUMBERS_LINES);
		setColumnsSize(DEFAULT_NUMBERS_COLUMNS);
	}
#endif
       return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(scilines)(int *nblines, int *nbcolumns)
{
	return scilines(*nblines,*nbcolumns);
}
/*--------------------------------------------------------------------------*/
int C2F(scilinesdefault)(void)
{
	return scilinesdefault();
}
/*--------------------------------------------------------------------------*/
BOOL setColumnsSize(int colums)
{
	C2F(iop).lct[4] = Max(MIN_NUMBERS_COLUMNS,colums);
	return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL setLinesSize(int lines_)
{

	C2F(iop).lct[1] = Max(MIN_NUMBERS_LINES,lines_);
	return TRUE;
}
/*--------------------------------------------------------------------------*/
int getColumnsSize(void)
{
	return C2F(iop).lct[4];
}
/*--------------------------------------------------------------------------*/
int getLinesSize(void)
{
	return C2F(iop).lct[1];
}
/*--------------------------------------------------------------------------*/
