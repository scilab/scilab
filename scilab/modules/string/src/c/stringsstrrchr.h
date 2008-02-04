/*----------------------------------------------------------------------------*/
/* Copyright INRIA 2007 */
/* @Author : Allan CORNET */
/*----------------------------------------------------------------------------*/
#ifndef __STRINGSTRRCHAR_H__
#define __STRINGSTRRCHAR_H__

#include "BOOL.h"

/*
* Locate last occurrence of character in string or
* Locate first occurrence of character in string
* @param[in] String matrix One
* @param[in] Dimension m*n of String matrix one
* @param[in] String matrix Two
* @param[in] Dimension m*n of String matrix two
* @param[in] BOOL to do strrchr
* @return a string matrix (result of strchr)
*/
char ** strings_strrchr(char **InputStrings,int Dim_InputStrings,char** InputChar,int Dim_InputChar,BOOL do_strrchr);

#endif /* __STRINGSTRRCHAR_H__ */
/*----------------------------------------------------------------------------*/


