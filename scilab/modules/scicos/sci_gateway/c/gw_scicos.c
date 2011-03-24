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
#include <string.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "api_common.h"
#include "callFunctionFromGateway.h"


/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{
	{sci_var2vec,"var2vec"},
	{sci_vec2var,"vec2var"},
	{sci_getblocklabel,"getblocklabel"},
	{sci_scicos_debug,"scicos_debug"},
	{sci_scicos_debug_count,"scicos_debug_count"},
	{sci_sctree,"sctree"},
	{sci_ftree2,"sci_tree2"},
	{sci_ftree3,"sci_tree3"},
	{sci_ftree4,"sci_tree4"},
	{sci_curblock,"curblock"},
	{sci_scicos_time,"scicos_time"},
	{sci_duplicate,"duplicate"},
	{sci_diffobjs,"diffobjs"},
	{sci_pointer_xproperty,"pointer_xproperty"},
	{sci_phase_simulation,"phase_simulation"},
	{sci_set_xproperty,"set_xproperty"},
	{sci_set_blockerror,"set_blockerror"},
	{sci_ctree2,"ctree2"},
	{sci_ctree3,"ctree3"},
	{sci_ctree4,"ctree4"},
	{sci_scicosim,"scicosim"},
	{sci_getscicosvars,"getscicosvars"},
	{sci_curblockc,"curblockc"},
	{sci_buildouttb,"buildouttb"},
	{sci_permutobj,"permutobj"},
	{sci_end_scicosim,"end_scicosim"},
	{sci_coserror, "coserror"},
	{sci_haltscicos, "sci_haltscicos"},
	{sci_data2sig,"data2sig"},
	{sci_sig2data,"sig2data"},
	{sci_model2blk,"model2blk"},
	{sci_callblk,"callblk"},
	{sci_loadScicos,"loadScicos"}
};
/*--------------------------------------------------------------------------*/
int gw_scicos(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/

