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
#ifndef __DYNAMIC_GATEWAYS_H__
#define __DYNAMIC_GATEWAYS_H__

/**
* helptools gateway
* helptools is loaded dynamically at runtime
* load gw_helptools in SCI/modules/helptools/sci_gateway/gw_helptools.c
*/
int gw_dynamic_helptools(void);


/**
* optimization gateway
* optimization is loaded dynamically at runtime
* load gw_optimization in SCI/modules/optimization/sci_gateway/gw_optimization.c
*/
int gw_dynamic_optimization(void);

/**
* matio gateway
* matio is loaded dynamically at runtime
* load gw_matio in SCI/modules/matio/sci_gateway/gw_matio.c
*/
int gw_dynamic_matio(void);

/**
* UMFPACK gateway
* UMFPACK is loaded dynamically at runtime
* load gw_umfpack in SCI/modules/umfpack/sci_gateway/gw_umfpack.c
*/
int gw_dynamic_umfpack(void);

/**
* spreadsheet gateway
* spreadsheet is loaded dynamically at runtime
* load gw_spreadsheet in SCI/modules/spreadsheet/sci_gateway/gw_spreadsheet.c
*/
int gw_dynamic_spreadsheet(void);

/**
* sound gateway
* sound is loaded dynamically at runtime
* load gw_sound in SCI/modules/sound/sci_gateway/gw_sound.c
*/
int gw_dynamic_sound(void);

/**
* Scicos gateway
* Scicos is loaded dynamically at runtime
* load gw_scicos in SCI/modules/scicos/sci_gateway/gw_scicos.c
*/
int gw_dynamic_scicos(void);

/**
* randlib gateway
* randlib is loaded dynamically at runtime
* load gw_randlib in SCI/modules/randlib/sci_gateway/gw_randlib.c
*/
int gw_dynamic_randlib(void);

/**
* arnoldi gateway
* arnoldi is loaded dynamically at runtime
* load gw_arnoldi in SCI/modules/arnoldi/sci_gateway/gw_arnoldi.c
*/
int gw_dynamic_arnoldi(void);

/**
* metanet gateway
* metanet is loaded dynamically at runtime
* load gw_metanet in SCI/modules/metanet/sci_gateway/gw_metanet.c
*/
int gw_dynamic_metanet(void);

/**
* symbolic gateway
* symbolic is loaded dynamically at runtime
* load gw_symbolic in SCI/modules/symbolic/sci_gateway/gw_symbolic.c
*/
int gw_dynamic_symbolic(void);

#endif /* __DYNAMIC_GATEWAYS_H__ */
/*--------------------------------------------------------------------------*/

