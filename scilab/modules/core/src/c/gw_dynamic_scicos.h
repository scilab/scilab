/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_DYNAMIC_SCICOS_H__
#define __GW_DYNAMIC_SCICOS_H__

#include "machine.h"

/**
* Scicos gateway
* Scicos is loaded dynamically at runtime
* load gw_scicos in SCI/modules/scicos/sci_gateway/gw_scicos.c
*/

int gw_dynamic_scicos(void);

#endif /* __GW_DYNAMIC_SCICOS_H__ */
/*--------------------------------------------------------------------------*/

