/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __SCILABMODE_H__
#define __SCILABMODE_H__

#include "machine.h"

typedef enum {
	SCILAB_API=1, /* Scilab is launch as an API */
	SCILAB_STD, /* The standard Scilab (gui, plot ...) */
	SCILAB_NW, /* Scilab in command line with the plots */
	SCILAB_NWNI /* Scilab in command line without any graphics. What not mandataroy here */
} scilabMode;

/* by default mode is API */

/**
* Set scilab mode 
* @param new scilab mode
* @return TRUE is newmode is OK
*/
BOOL setScilabMode(scilabMode newmode);

/**
* Get scilab mode
* @return current scilab mode
*/
scilabMode getScilabMode(void);


/**
 * Gives the mode of scilab
 * @param[out] the mode of scilab
 * @return 1
 */
int C2F(getscilabmode)(scilabMode *mode);


#endif /* __SCILABMODE_H__ */
/*--------------------------------------------------------------------------*/ 
