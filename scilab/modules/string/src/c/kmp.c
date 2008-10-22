
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : The algorithm of kmp method */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "kmp.h"
/*------------------------------------------------------------------------*/
/**
 * The KMP method to search in a string.
 * @Main_String[] is  the input of the main string.
 * @Sub_String[] is  the input of substring.
 * @return  the start point of the substring or 0.
 */
int kmp(char Main_String[],char Sub_String[],int pos,int *next)
{
     int i = pos; /*The start point*/
	 int j = 0;
	 int lenS=(int)strlen(Main_String);  /*The length of the main string*/
	 int lenT= (int)strlen(Sub_String);  /*The length of the substring*/

     while( (i < lenS) && (j < lenT) )
     {
         if( (j == -1) || (Main_String[i] == Sub_String[j]) )
         {
              i++;           /*Compare with the char next*/
              j++;
         }
         else j = next[j];    /*Using the next pattern to move right*/
     }

     if( j >= lenT) return(i-lenT+1);
     else
     {
		 /*Should give an error message*/
         return(0);
     }
 
}
/*------------------------------------------------------------------------*/
/**
 * To get the next value of the substring of the KMP method.
 * @*next is the pointer to the next value.
 * @Substring[] is  the input of substring.
 */
int * getnext(char Substring[])
{                        
	/*To get the next value of the substring*/
	int i = 0,j = -1;
	int lenT = (int)strlen(Substring);
	int *next = (int *)MALLOC( sizeof(int) * ( lenT+1 ) );

	next[0]=-1;
	while(i < lenT)
	{
		if( (j==-1) || (Substring[i] == Substring[j]) )
		{  
			++i;
			++j;
			next[i]=j;
        }
        else j = next[j];
    }
	return next;
}
/*-----------------------------------------------------------------------------------*/
