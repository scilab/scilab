/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "completeLine_wrap_java.h"
#include "completeLine.h"
#include "getPartLine.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
char *completelineforjava(char *currentLine,char *stringToAdd,BOOL isFile, char *postCaretLine)
{
	char *assembledLine = NULL;
	char *patternFile = getFilePartLevel(currentLine);
	char *patternLine = getPartLevel(currentLine);

	assembledLine = completeLine(currentLine,stringToAdd,patternFile,patternLine,isFile, postCaretLine);
	if (patternFile) {FREE(patternFile); patternFile = NULL;}
	if (patternLine) {FREE(patternLine); patternLine = NULL;}

	return assembledLine;
}
/*--------------------------------------------------------------------------*/
