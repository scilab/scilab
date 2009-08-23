/*  Scicos
*
*  Copyright (C) INRIA - Allan CORNET
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __GW_SCICOS__
#define __GW_SCICOS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_scicos(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */
/*--------------------------------------------------------------------------*/
int C2F(sci_var2vec)(char *fname,unsigned long fname_len);
int C2F(sci_vec2var)(char *fname,unsigned long fname_len);
int C2F(sci_getblocklabel)(char *fname,unsigned long fname_len);
int C2F(sci_scicos_debug)(char *fname,unsigned long fname_len);
int C2F(sci_scicos_debug_count)(char *fname,unsigned long fname_len);
int C2F(sci_sctree)(char *fname,unsigned long fname_len);
int C2F(sci_sci_tree2)(char *fname,unsigned long fname_len);
int C2F(sci_sci_tree3)(char *fname,unsigned long fname_len);
int C2F(sci_sci_tree4)(char *fname,unsigned long fname_len);
int C2F(sci_curblock)(char *fname,unsigned long fname_len);

int C2F(sci_scicos_time)(char *fname,unsigned long fname_len);
int C2F(sci_duplicate)(char *fname,unsigned long fname_len);
int C2F(sci_diffobjs)(char *fname,unsigned long fname_len);
int C2F(sci_pointer_xproperty)(char *fname,unsigned long fname_len);
int C2F(sci_phase_simulation)(char *fname,unsigned long fname_len);
int C2F(sci_set_xproperty)(char *fname,unsigned long fname_len);
int C2F(sci_set_blockerror)(char *fname,unsigned long fname_len);
int C2F(sci_ctree2)(char *fname,unsigned long fname_len);
int C2F(sci_ctree3)(char *fname,unsigned long fname_len);
int C2F(sci_ctree4)(char *fname,unsigned long fname_len);
int C2F(sci_scicosim)(char *fname,unsigned long fname_len);
int C2F(sci_getscicosvars)(char *fname,unsigned long fname_len);
int C2F(sci_curblockc)(char *fname,unsigned long fname_len);
int C2F(sci_buildouttb)(char *fname,unsigned long fname_len);
int C2F(sci_permutobj)(char *fname,unsigned long fname_len);
int C2F(sci_end_scicosim)(char *fname,unsigned long fname_len);
int sci_haltscicos(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SCICOS__ */
/*--------------------------------------------------------------------------*/
