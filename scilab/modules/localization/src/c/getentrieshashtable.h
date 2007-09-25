/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETENTRIESHASHTABLE_H__
#define __GETENTRIESHASHTABLE_H__

#include <string.h>
#include "hashtable.h"

/* get a array of strings from hastable tableIN */
char **getTAGSinhashtable(struct hashtable *tableIN,int *numbersEntries);
char **getSTRINGSinhashtable(struct hashtable *tableIN,int *numbersEntries);
char **getPATHSinhashtable(struct hashtable *tableIN,int *numbersEntries);

#endif
/*-----------------------------------------------------------------------------------*/
