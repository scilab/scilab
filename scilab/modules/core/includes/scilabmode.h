/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCILABMODE_H__
#define __SCILABMODE_H__

#include "machine.h"



typedef enum {SCILAB_API=1,SCILAB_STD,SCILAB_NW,SCILAB_NWNI} scilabMode;

/* by default mode is API */

/**
* Set scilab mode 
* param new scilab mode
* return TRUE is newmode is OK
*/
BOOL setScilabMode(int newmode);

/**
* Get scilab mode
* return current scilab mode
*/
int getScilabMode(void);


/**
* @return 1
*/
int C2F(getscilabmode)(int *mode);


#endif /* __SCILABMODE_H__ */
/*-----------------------------------------------------------------------------------*/ 
