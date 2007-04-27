/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __GENMDSP_H__
#define __GENMDSP_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param x
 * @param nx
 * @param mm
 * @param nn
 * @param ll
 * @param lunit
 * @param cw
 * @param iw
 * @param cw_len
 * @return 
 */
int C2F(genmdsp)(integer *typ, integer *x, integer *nx, integer *mm, integer *nn, integer *ll, integer *lunit, char cw[], integer *iw, int cw_len);
#endif /* __GENMDSP_H__ */
