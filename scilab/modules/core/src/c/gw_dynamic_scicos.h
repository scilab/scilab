/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __GW_DYNAMIC_SCICOS_H__
#define __GW_DYNAMIC_SCICOS_H__

#include "machine.h"

/**
* Scicos gateway
* Scicos is loaded dynamically at runtime
* load C2F(gw_scicos) in SCI/modules/scicos/sci_gateway/gw_scicos.c
*/

int C2F(gw_dynamic_scicos)(void);

#endif /* __GW_DYNAMIC_SCICOS_H__ */
/*--------------------------------------------------------------------------*/