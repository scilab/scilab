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
#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
int gw_pvm(void);
/*--------------------------------------------------------------------------*/ 
int intspvm_addhosts(char *fname,unsigned long fname_len);
int intspvm_barrier(char *fname,unsigned long fname_len);
int intspvm_bcast(char *fname,unsigned long fname_len);
int intspvm_bufinfo(char *fname,unsigned long fname_len);
int intspvm_config(char *fname,unsigned long fname_len);
int intspvm_delhosts(char *fname,unsigned long fname_len);
int intspvm_error(char *fname,unsigned long fname_len);
int intspvm_error_mode(char *fname,unsigned long fname_len);
int intspvm_exit(char *fname,unsigned long fname_len);
int intspvm_f772sci(char *fname,unsigned long fname_len);
int intspvm_getinst(char *fname,unsigned long fname_len);
int intspvm_gettid(char *fname,unsigned long fname_len);
int intspvm_get_timer(char *fname,unsigned long fname_len);
int intspvm_gsize(char *fname,unsigned long fname_len);
int intspvm_halt(char *fname,unsigned long fname_len);
int intspvm_joingroup(char *fname,unsigned long fname_len);
int intspvm_kill(char *fname,unsigned long fname_len);
int intspvm_lvgroup(char *fname,unsigned long fname_len);
int intspvm_mytid(char *fname,unsigned long fname_len);
int intspvm_parent(char *fname,unsigned long fname_len);
int intspvm_probe(char *fname,unsigned long fname_len);
int intspvm_recv(char *fname,unsigned long fname_len);
int intspvm_recv_var(char *fname,unsigned long fname_len);
int intspvm_reduce(char *fname,unsigned long fname_len);
int intspvm_sci2f77(char *fname,unsigned long fname_len);
int intspvm_send(char *fname,unsigned long fname_len);
int intspvm_send_var(char *fname,unsigned long fname_len);
int intspvm_set_timer(char *fname,unsigned long fname_len);
int intspvm_spawn(char *fname,unsigned long fname_len);
int intspvm_spawn_independent(char *fname,unsigned long fname_len);
int intspvm_start(char *fname,unsigned long fname_len);
int intspvm_tasks(char *fname,unsigned long fname_len);
int intspvm_tidtohost(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __INTMATPVM__ */
/*--------------------------------------------------------------------------*/
