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

#include <string.h>
#include "gw_pvm.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]={
  {intspvm_joingroup,"pvm_joingroup"},
  {intspvm_lvgroup,"pvm_lvgroup"},
  {intspvm_gsize,"pvm_gsize"},
  {intspvm_gettid,"pvm_gettid"},
  {intspvm_getinst,"pvm_getinst"},
  {intspvm_barrier,"pvm_barrier"},
  {intspvm_bcast,"pvm_bcast"},
  {intspvm_tasks,"pvm_tasks"},
  {intspvm_config,"pvm_config"},
  {intspvm_addhosts,"pvm_addhosts"},
  {intspvm_delhosts,"pvm_delhosts"},
  {intspvm_parent,"pvm_parent"},
  {intspvm_tidtohost,"pvm_tidtohost"},
  {intspvm_set_timer,"pvm_set_timer"},
  {intspvm_get_timer,"pvm_get_timer"},
  {intspvm_mytid,"pvm_mytid"},
  {intspvm_exit,"pvm_exit"},
  {intspvm_kill,"pvm_kill"},
  {intspvm_spawn,"pvm_spawn"},
  {intspvm_spawn_independent,"pvm_spawn_independent"},
  {intspvm_recv,"pvm_recv"},
  {intspvm_send,"pvm_send"},
  {intspvm_recv_var,"pvm_recv_var"},
  {intspvm_send_var,"pvm_send_var"},
  {intspvm_reduce,"pvm_reduce"},
  {intspvm_start,"pvm_start"},
  {intspvm_halt,"pvm_halt"},
  {intspvm_error,"pvm_error"},
  {intspvm_sci2f77,"pvm_sci2f77"},
  {intspvm_f772sci,"pvm_f772sci"},
  {intspvm_probe,"pvm_probe"},
  {intspvm_bufinfo,"pvm_bufinfo"},
  {intspvm_error_mode,"pvm_error_mode"},
};
/*--------------------------------------------------------------------------*/
int gw_pvm(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
