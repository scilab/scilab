/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "getfulldictionary.h"
#include "getvariablesname.h"
#include "commandwords.h"
#include "getfunctionslist.h"
#include "getmacrosdictionary.h"
#include "toolsdictionary.h"
#include "MALLOC.h"
#include "getDictionarySetProperties.h"
#include "getDictionaryGetProperties.h"
/*--------------------------------------------------------------------------*/
char **getfulldictionary(int *sizearray)
{
	int sizedictionary = 0;
	char **dictionary = NULL;
	
	int sizevariablesdictionary = 0;
	char **variablesdictionary = getVariablesName(&sizevariablesdictionary,TRUE);
	
	int sizecommandsdictionary = 0;
	char **commandsdictionary = getcommandkeywords(&sizecommandsdictionary);
	
	int sizefunctionsdictionary = 0;
	char **functionsdictionary = GetFunctionsList(&sizefunctionsdictionary);
	
	int sizemacrosdictionary = 0;
	char **macrosdictionary = getmacrosdictionary(&sizemacrosdictionary);

	int sizeHandleGraphicsGetPropertiesDictionary = 0;
	char **HandleGraphicsGetPropertiesDictionary = getDictionaryGetProperties(&sizeHandleGraphicsGetPropertiesDictionary);

	int sizeHandleGraphicsSetPropertiesDictionary = 0;
	char **HandleGraphicsSetPropertiesDictionary = getDictionarySetProperties(&sizeHandleGraphicsSetPropertiesDictionary);
	
	sizedictionary = sizevariablesdictionary + sizecommandsdictionary + sizefunctionsdictionary
					+ sizemacrosdictionary + sizeHandleGraphicsGetPropertiesDictionary
					+ sizeHandleGraphicsSetPropertiesDictionary;

	if (sizedictionary > 0) dictionary = (char**)MALLOC(sizeof(char*)*sizedictionary);

	if (dictionary)
	{
		int i = 0;

		appendDictionary(&dictionary,&i,&variablesdictionary,&sizevariablesdictionary);
		appendDictionary(&dictionary,&i,&commandsdictionary,&sizecommandsdictionary);
		appendDictionary(&dictionary,&i,&functionsdictionary,&sizefunctionsdictionary);
		appendDictionary(&dictionary,&i,&macrosdictionary,&sizemacrosdictionary);
		appendDictionary(&dictionary,&i,&HandleGraphicsGetPropertiesDictionary,&sizeHandleGraphicsGetPropertiesDictionary);
		appendDictionary(&dictionary,&i,&HandleGraphicsSetPropertiesDictionary,&sizeHandleGraphicsSetPropertiesDictionary);

		dictionary = RemoveDuplicateDictionary(dictionary,&i);

		*sizearray = i;
	}
	else
	{
		*sizearray = 0;
	}
	return dictionary;
}
/*--------------------------------------------------------------------------*/
