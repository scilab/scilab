/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETDATE_H__
#define __GETDATE_H__

#ifdef _MSC_VER
#include <sys/types.h> 
#include <sys/timeb.h>
#else
#include <sys/time.h> 
#endif

#include "machine.h"
/**
 * TODO : comment
 * @param dt
 * @param ierr
 */
void C2F(scigetdate) __PARAMS ((time_t *dt, int *ierr));


/**
 * TODO : comment
 * @param dt
 * @param w
 */
void C2F(convertdate) __PARAMS ((time_t *dt, int w[]));

#endif /* __GETDATE_H__ */
