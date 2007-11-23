/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __SCILINES_H__
#define __SCILINES_H__

#include "machine.h" /* C2F */

/**
* set number of lines and columns to display data
* @param[in] number of lines
* @param[in] number of columns
* @return 0
*/
int scilines(int nl, int nc);

/**
* set default number of lines and columns to display data
* @return 0
*/
int scilinesdefault(void);

/**
* set number of lines and columns to display data (C2F version)
* @param[in] number of lines
* @param[in] number of columns
* @return 0
*/
int C2F(scilines)(int *nl, int *nc);

/**
* set default number of lines and columns to display data (C2F version)
* @return 0
*/
int C2F(scilinesdefault)(void);

/**
* set number of columns to display data
* @param new size of columns
* @return TRUE or FALSE
*/ 
BOOL setColumnsSize(int colums);

/**
* set number of lines to display data
* @param new size of lines
* @return TRUE or FALSE
*/ 
BOOL setLinesSize(int lines);

/**
* get number of columns to display data
* @return numbers of columns
*/ 
int getColumnsSize(void);

/**
* get number of lines to display data
* @return numbers of lines
*/ 
int getLinesSize(void);

#endif /* __SCILINES_H__ */
/*--------------------------------------------------------------------------*/
