/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __SET_XXPRINTF_H__
#define __SET_XXPRINTF_H__
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "PARAMS.h"
#include "stack-def.h" /* bsiz */
/*-----------------------------------------------------------------------------------*/
typedef int (*XXPRINTF) __PARAMS((FILE *, char *,...));
typedef int (*FLUSH) __PARAMS((FILE *));
/*-----------------------------------------------------------------------------------*/
#define  MAX_SPRINTF_SIZE  bsiz
/*-----------------------------------------------------------------------------------*/

/**
* set output function  sprintf, sciprint2, fprintf
* @param[in] fp "Pointer to a FILE object"
* @param[out] xxprintf "Pointer to output function"
* @param[in] flush flush
* @param[out] target output "buffer"
*/
void set_xxprintf(FILE *fp,XXPRINTF *xxprintf,FLUSH *flush,char **target);

#endif /* __SET_XXPRINTF_H__ */
/*-----------------------------------------------------------------------------------*/

