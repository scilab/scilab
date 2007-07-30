/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "commandwords.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
/* if you update command.f , please update this file too ... */
/*-----------------------------------------------------------------------------------*/ 
static const int nbrCommands=29;
/*-----------------------------------------------------------------------------------*/ 
static char *CommandWords[]={
 "if", "else",
 "for", "while",
 "end", "select",
 "case", "quit",
 "return", "help",
 "what", "who",
 "pause", "clear",
 "resume", "then",
 "do", "apropos",
 "abort", "break",
 "elseif", "pwd",
 "function", "endfunction",
 "clc", "continue",
 "try", "catch",
 "exit"
};
/*-----------------------------------------------------------------------------------*/ 
static void SortStrings(char **Strs,int SizeOfStrs);
/*-----------------------------------------------------------------------------------*/ 
char **getcommandkeywords(int *sizearray)
{
	char **keywords = NULL;

	keywords = (char**)MALLOC(sizeof(char*)*nbrCommands);

	if (keywords)
	{
		int i = 0;
		for (i = 0;i < nbrCommands; i++)
		{
			char *commandword = NULL;
			commandword = (char*)MALLOC(sizeof(char)*(strlen(CommandWords[i])+1));
			if (commandword) 
			{
				strcpy(commandword,CommandWords[i]);
				keywords[i] = commandword;
			}
		}
		*sizearray =  nbrCommands;

		SortStrings(keywords,*sizearray);
	}
	else
	{
		*sizearray = 0;
	}

	return keywords;
}
/*-----------------------------------------------------------------------------------*/ 
static void SortStrings(char **Strs,int SizeOfStrs)
{
	int fin,i;
	for(fin=SizeOfStrs-1;fin>0;fin--)
	{
		int Sorted=FALSE;
		for(i=0;i<fin;i++)
		{
			if(strcmp(Strs[i],Strs[i+1])>0)
			{
				char *tmp;
				memcpy(&tmp,Strs+i,sizeof(char *));
				memcpy(Strs+i,Strs+(i+1),sizeof(char *));
				memcpy(Strs+(i+1),&tmp,sizeof(char *));
				Sorted=TRUE;
			}
		}
		if(!Sorted)break;
	}
}
/*-----------------------------------------------------------------------------------*/
