/*------------------------------------------------------------------------*/
/* File: stripblanks.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu , A.C                                               */
/* desc : The algorithm of stripblanks                                    */                                    
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "MALLOC.h" 
#include "stripblanks.h"
/*------------------------------------------------------------------------*/
static char* substr(const char*str, unsigned start, unsigned end);
/*------------------------------------------------------------------------*/
#define BLANK_CHARACTER ' '
#define TAB_CHARACTER 9
/*------------------------------------------------------------------------*/
static char* substr(const char*str, unsigned start, unsigned end)
{
	unsigned n = end - start;
	char *stbuf = NULL;
    stbuf = (char*)MALLOC(sizeof(char*)*(n+1));
	
	if (stbuf)
	{
		if ( strcmp(str,"") != 0 )
		{
			strncpy(stbuf, str + start, n);   /*Put a part of str into stbuf*/
			stbuf[n] = 0;
		}
		else strcpy(stbuf,"");
	}
	return stbuf;
}
/*------------------------------------------------------------------------*/
void stripblanks(char **InputStrings,char **OutputStrings,int InputStringsDim,BOOL withTAB)
{
    int x = 0;
	char *replacedstr = NULL;

	for ( x = 0; x < InputStringsDim ; x++)   /*Search for all the matrix*/
	{
		int lenghtInput_One = 0;
		int lenghtInput_Two = 0;
		int i = 0;
		
		/* search character ' ' or TAB from end of the string */
		for ( i = (int)strlen(InputStrings[x]) - 1 ; i > 0 ; i--)
		{
			if (withTAB)
			{
				if ( (InputStrings[x][i] == (char)BLANK_CHARACTER) ||
					 (InputStrings[x][i] == (char)TAB_CHARACTER) )
					 continue;
				else
				{
					lenghtInput_Two = i;
					break;
				}
			}
			else
			{
				if (InputStrings[x][i] == (char)BLANK_CHARACTER)	continue;
				else
				{
					lenghtInput_Two = i;
					break;
				}
			}
		}
		/* search character ' ' or TAB from beginning of the string */
		if (lenghtInput_Two > 0) for (i = 0;i<(int)strlen(InputStrings[x]); i++)
		{
			if (withTAB)
			{
				if ( (InputStrings[x][i] == (char)BLANK_CHARACTER) ||
					(InputStrings[x][i] == (char)TAB_CHARACTER) )
					continue;
				else
				{
					lenghtInput_One = i;
					break;
				}
			}
			else
			{
				if (InputStrings[x][i] == (char)BLANK_CHARACTER)	continue;
				else
				{
					lenghtInput_One = i;
					break;
				}
			}
		}

		if (lenghtInput_One <= lenghtInput_Two)
		{
			/*Get the substring without tabs*/ 
			replacedstr=substr(InputStrings[x], lenghtInput_One, lenghtInput_Two+1 ); 
			/*To add the substring into the output matrix*/
			strcpy(OutputStrings[x],replacedstr);
		}
		else
		{
			strcpy(OutputStrings[x],InputStrings[x]);
		}
	}
}
/*------------------------------------------------------------------------*/
