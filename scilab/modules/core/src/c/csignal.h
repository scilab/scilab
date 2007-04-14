/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __CSIGNAL_H__ 
#define __CSIGNAL_H__
#include <signal.h>
#include "machine.h"

/** 
 * TODO : comment
 * @param sig
 */
void controlC_handler (int sig);


/** 
 * TODO : comment
 */
int C2F(csignal)(void);

#endif /* __CSIGNAL_H__ */