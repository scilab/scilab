/*-----------------------------------------------------------------------------------*/
/* Author : Pierre MARECHAL                                                          */
/* Scilab team                                                                       */
/* Copyright INRIA                                                                   */
/*                                                                                   */
/* Date : 24 august 2006                                                             */
/*-----------------------------------------------------------------------------------*/

#ifndef __INTGETRELATIVEFILENAME__
#define __INTGETRELATIVEFILENAME__

#ifdef _MSC_VER
	#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"

#ifndef NULL
	#define NULL 0
#endif

#ifdef _MSC_VER
	#define ABSOLUTE_NAME_START 3
#else
	#define ABSOLUTE_NAME_START 1
#endif

// ABSOLUTE_NAME_START
// 
// The number of characters at the start of an absolute filename.  e.g. in DOS,
// absolute filenames start with "X:\" so this value should be 3, in UNIX they start
// with "\" so this value should be 1.

int C2F(sci_getrelativefilename) _PARAMS((char *fname, unsigned long l));
char* getrelativefilename(char *currentDirectory, char *absoluteFilename);

#endif /* __INTGETRELATIVEFILENAME__ */
/*-----------------------------------------------------------------------------------*/ 
