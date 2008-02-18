
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __KMP_H__
#define __KMP_H__

/*------------------------------------------------------------------------*/
/**
 * The KMP method to search in a string.
 * @Main_String[] is  the input of the main string.
 * @Sub_String[] is  the input of substring.
 * @return  the start point of the substring or 0.
 */
int kmp(char Main_String[],char Sub_String[],int pos,int *next);
/*------------------------------------------------------------------------*/
/**
 * To get the next value of the substring of the KMP method.
 * @*next is the pointer to the next value.
 * @Substring[] is  the input of substring.
 */
int * getnext(char Substring[]);
#endif /* __KMP_H__ */
/*------------------------------------------------------------------------*/
