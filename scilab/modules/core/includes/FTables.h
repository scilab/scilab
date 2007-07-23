/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#ifndef __FTABLES__
#define __FTABLES__
#include "machine.h"

typedef void (*voidf)();

typedef struct {
  char *name;
  voidf f;
} FTAB;

/**
 * @TODO : add comment
 *
 * @param name  
 * @param rep   
 * @param table 
 * @return <ReturnValue>
 */
voidf SetFunction  __PARAMS((char *name, int *rep, FTAB *table));  

#endif /*__FTABLES__*/
