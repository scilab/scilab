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
  {sci_pvm_joingroup,"pvm_joingroup"},
  {sci_pvm_lvgroup,"pvm_lvgroup"},
  {sci_pvm_gsize,"pvm_gsize"},
  {sci_pvm_gettid,"pvm_gettid"},
  {sci_pvm_getinst,"pvm_getinst"},
  {sci_pvm_barrier,"pvm_barrier"},
  {sci_pvm_bcast,"pvm_bcast"},
  {sci_pvm_tasks,"pvm_tasks"},
  {sci_pvm_config,"pvm_config"},
  {sci_pvm_addhosts,"pvm_addhosts"},
  {sci_pvm_delhosts,"pvm_delhosts"},
  {sci_pvm_parent,"pvm_parent"},
  {sci_pvm_tidtohost,"pvm_tidtohost"},
  {sci_pvm_set_timer,"pvm_set_timer"},
  {sci_pvm_get_timer,"pvm_get_timer"},
  {sci_pvm_mytid,"pvm_mytid"},
  {sci_pvm_exit,"pvm_exit"},
  {sci_pvm_kill,"pvm_kill"},
  {sci_pvm_spawn,"pvm_spawn"},
  {sci_pvm_spawn_independent,"pvm_spawn_independent"},
  {sci_pvm_recv,"pvm_recv"},
  {sci_pvm_send,"pvm_send"},
  {sci_pvm_recv_var,"pvm_recv_var"},
  {sci_pvm_send_var,"pvm_send_var"},
  {sci_pvm_reduce,"pvm_reduce"},
  {sci_pvm_start,"pvm_start"},
  {sci_pvm_halt,"pvm_halt"},
  {sci_pvm_error,"pvm_error"},
  {sci_pvm_sci2f77,"pvm_sci2f77"},
  {sci_pvm_f772sci,"pvm_f772sci"},
  {sci_pvm_probe,"pvm_probe"},
  {sci_pvm_bufinfo,"pvm_bufinfo"},
  {sci_pvm_error_mode,"pvm_error_mode"},
};
/*--------------------------------------------------------------------------*/
int gw_pvm(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
