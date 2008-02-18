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

#ifndef __INTMATPVM__
#define __INTMATPVM__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_pvm)(void);
/*--------------------------------------------------------------------------*/ 
int intspvm_addhosts _PARAMS((char *fname,unsigned long fname_len));
int intspvm_barrier _PARAMS((char *fname,unsigned long fname_len));
int intspvm_bcast _PARAMS((char *fname,unsigned long fname_len));
int intspvm_bufinfo _PARAMS((char *fname,unsigned long fname_len));
int intspvm_config _PARAMS((char *fname,unsigned long fname_len));
int intspvm_delhosts _PARAMS((char *fname,unsigned long fname_len));
int intspvm_error _PARAMS((char *fname,unsigned long fname_len));
int intspvm_error_mode _PARAMS((char *fname,unsigned long fname_len));
int intspvm_exit _PARAMS((char *fname,unsigned long fname_len));
int intspvm_f772sci _PARAMS((char *fname,unsigned long fname_len));
int intspvm_getinst _PARAMS((char *fname,unsigned long fname_len));
int intspvm_gettid _PARAMS((char *fname,unsigned long fname_len));
int intspvm_get_timer _PARAMS((char *fname,unsigned long fname_len));
int intspvm_gsize _PARAMS((char *fname,unsigned long fname_len));
int intspvm_halt _PARAMS((char *fname,unsigned long fname_len));
int intspvm_joingroup _PARAMS((char *fname,unsigned long fname_len));
int intspvm_kill _PARAMS((char *fname,unsigned long fname_len));
int intspvm_lvgroup _PARAMS((char *fname,unsigned long fname_len));
int intspvm_mytid _PARAMS((char *fname,unsigned long fname_len));
int intspvm_parent _PARAMS((char *fname,unsigned long fname_len));
int intspvm_probe _PARAMS((char *fname,unsigned long fname_len));
int intspvm_recv _PARAMS((char *fname,unsigned long fname_len));
int intspvm_recv_var _PARAMS((char *fname,unsigned long fname_len));
int intspvm_reduce _PARAMS((char *fname,unsigned long fname_len));
int intspvm_sci2f77 _PARAMS((char *fname,unsigned long fname_len));
int intspvm_send _PARAMS((char *fname,unsigned long fname_len));
int intspvm_send_var _PARAMS((char *fname,unsigned long fname_len));
int intspvm_set_timer _PARAMS((char *fname,unsigned long fname_len));
int intspvm_spawn _PARAMS((char *fname,unsigned long fname_len));
int intspvm_spawn_independent _PARAMS((char *fname,unsigned long fname_len));
int intspvm_start _PARAMS((char *fname,unsigned long fname_len));
int intspvm_tasks _PARAMS((char *fname,unsigned long fname_len));
int intspvm_tidtohost _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __INTMATPVM__ */
/*--------------------------------------------------------------------------*/
