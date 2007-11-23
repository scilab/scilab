/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENIMPL_H__
#define __GENIMPL_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param last
 * @param res
 * @return 
 */
int C2F(genimpl2)(integer *typ,integer *first,integer *last,integer *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return 
 */
int C2F(genimpl3)(integer *typ,integer *first,integer *step,integer *last,integer *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param last
 * @param res
 * @return 
 */
int C2F(genimpl2dim)(integer *typ,integer *first,integer *last,integer *res);

/**
 * TODO : comment 
 * @param typ
 * @param first
 * @param step
 * @param last
 * @param res
 * @return 
 */
int C2F(genimpl3dim)(integer * typ,integer * first,integer * step,integer * last,integer * res);

#endif /* __GENIMPL_H__ */
