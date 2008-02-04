/*------------------------------------------------------------------------*/
/* Copyright INRIA 2007*/
/*------------------------------------------------------------------------*/
#ifndef __STRSPLIT_H__
#define __STRSPLIT_H__

/**
* string split split a string into a vector of strings 
* @param[in] Input_StringMatrix a string 
* @param[in] Input_IntMatrix a vector of stricly increasing indices in the interval  [1 length(str)-1]
* @param[out]Output_StringMatrix the resulting column vector of strings (dimension  size(ind,'*')+1  ).  
*@return
*/

void strsplit(char **Input_StringMatrix,char **Output_StringMatrix,int *Row_Pointer,int *Row_Pointer_Two,int *Col_Pointer,int *Input_IntMatrix);

#endif /* __STRSPLIT_H__ */
/*------------------------------------------------------------------------*/
