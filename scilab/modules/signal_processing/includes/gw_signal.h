/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "dynlib_signal_processing.h"
#include "machine.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
SIGNAL_PROCESSING_IMPEXP int gw_signal_processing(void);
/*--------------------------------------------------------------------------*/

SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_remez);
//SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_rpem); => c++
//SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_corr); -> c++
//SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_fft); -> c++
SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_amell);
//SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_delip); -> c++
//SIGNAL_PROCESSING_IMPEXP C_GATEWAY_PROTOTYPE(sci_syredi); -> c++

/*
//YaSp
SIGNAL_PROCESSING_IMPEXP int sci_remez(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_rpem(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_corr(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_ffir(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_fft(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_fiir(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_amell(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_delip(char *fname, int* _piKey);
SIGNAL_PROCESSING_IMPEXP int sci_syredi(char *fname, int* _piKey);
*/
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

