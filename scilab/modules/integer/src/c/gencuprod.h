/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __GENCUPROD_H__
#define __GENCUPROD_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param n
 * @param w be carreful using this parameter (crappy way of doing polymorphism)
 * @param iw
 * @return 
 */
int C2F(gencuprod)(integer *typ,integer *n, void *w, integer *iw);

#endif /* __GENCUPROD_H__ */
