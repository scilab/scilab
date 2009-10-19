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
* xcos gateway
* xcos is loaded dynamically at runtime
* load gw_xcos in SCI/modules/xcos/sci_gateway/gw_xcos.c
*/
int gw_dynamic_xcos(void);

/**
* xpad gateway
* functions is loaded dynamically at runtime
* load gw_functions in SCI/modules/xpad/sci_gateway/gw_xpad.c
*/
int gw_dynamic_xpad(void);

/**
* functions gateway
* functions is loaded dynamically at runtime
* load gw_functions in SCI/modules/functions/sci_gateway/gw_functions.c
*/
int gw_dynamic_functions(void);

/**
* interpolation gateway
* interpolation is loaded dynamically at runtime
* load gw_interpolation in SCI/modules/interpolation/sci_gateway/gw_interpolation.c
*/
int gw_dynamic_interpolation(void);

/**
* statistics gateway
* statistics is loaded dynamically at runtime
* load gw_statistics in SCI/modules/statistics/sci_gateway/gw_statistics.c
*/
int gw_dynamic_statistics(void);


/**
* signal_processing gateway
* signal_processing is loaded dynamically at runtime
* load gw_signal_processing in SCI/modules/signal_processing/sci_gateway/gw_signal_processing.c
*/
int gw_dynamic_signal_processing(void);


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

/**
* hdf5 gateway
* hdf5 is loaded dynamically at runtime
* load gw_symbolic in SCI/modules/hdf5/sci_gateway/gw_hdf5.c
*/
int gw_dynamic_hdf5(void);

#endif /* __DYNAMIC_GATEWAYS_H__ */
/*--------------------------------------------------------------------------*/

