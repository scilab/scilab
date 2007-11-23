/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __SPBUILD_H__
#define __SPBUILD_H__

#include "spDefs.h"
/* TODO: move comments from spBuild.c */

ElementPtr spcFindElementInCol( MatrixPtr Matrix, register ElementPtr *LastAddr, register int Row, int Col, SPBOOLEAN CreateIfMissing );

ElementPtr spcCreateElement( MatrixPtr Matrix, int Row, register int Col, register ElementPtr  * LastAddr, SPBOOLEAN Fillin );

int spcLinkRows( MatrixPtr Matrix );

#endif /* __SPBUILD_H__ */
