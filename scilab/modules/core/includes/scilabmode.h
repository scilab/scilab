/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCILABMODE_H__
#define __SCILABMODE_H__

#include "machine.h"

/* standard mode by default is STD mode */
/* jvm , events loop (tcl/tk) , ... */ 

/**
* set STD mode
* @return 
*/
BOOL setSTDmode(void);

/**
* set NWNI mode
* @return 
*/
BOOL setNWNImode(void);

/**
* set NW mode
* @return 
*/
BOOL setNWmode(void);

/**
* is STD mode ?
* @return TRUE or FALSE
*/
BOOL isSTDmode(void);

/**
* is NW mode ?
* @return TRUE or FALSE
*/
BOOL isNWmode(void);

/**
* is NWNI mode ?
* @return TRUE or FALSE
*/
BOOL isNWNImode(void);

/**
* is STD mode ? C2F version (fortran)
* @param ok 0 --> FALSE 1--> TRUE 
* @return 1
*/
int C2F(isstdmode)(int *ok);

/**
* is NW mode ? C2F version (fortran)
* @param ok 0 --> FALSE 1--> TRUE 
* @return 1
*/
BOOL C2F(isnwmode)(int *ok);

/**
* is NWNI mode ? C2F version (fortran)
* @param ok 0 --> FALSE 1--> TRUE 
* @return 1
*/
BOOL C2F(isnwnimode)(int *ok);

#endif /* __SCILABMODE_H__ */
/*-----------------------------------------------------------------------------------*/ 
