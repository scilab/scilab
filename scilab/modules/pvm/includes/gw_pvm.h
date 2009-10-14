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
#include "dynlib_pvm.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
PVM_IMPEXP int gw_pvm(void);
/*--------------------------------------------------------------------------*/ 
PVM_IMPEXP int sci_pvm_addhosts(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_barrier(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_bcast(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_bufinfo(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_config(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_delhosts(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_error(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_error_mode(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_exit(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_f772sci(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_getinst(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_gettid(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_get_timer(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_gsize(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_halt(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_joingroup(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_kill(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_lvgroup(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_mytid(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_parent(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_probe(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_recv(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_recv_var(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_reduce(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_sci2f77(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_send(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_send_var(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_set_timer(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_spawn(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_spawn_independent(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_start(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_tasks(char *fname,unsigned long fname_len);
PVM_IMPEXP int sci_pvm_tidtohost(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __INTMATPVM__ */
/*--------------------------------------------------------------------------*/
