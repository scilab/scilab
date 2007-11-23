/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __WITH_FFTW_H__
#define __WITH_FFTW_H__

#include "machine.h" /* C2F , BOOL*/

/** 
* check if it is with FFTW
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
int C2F(withfftw)(int *rep);

/** 
* check if it is with FFTW
* @return TRUE or FALSE
*/
BOOL withfftw(void);

#endif /* __WITH_FFTW_H__ */
/*--------------------------------------------------------------------------*/ 
