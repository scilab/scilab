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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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
#include "dynlib_scicos.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
SCICOS_IMPEXP int gw_scicos(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */
/*--------------------------------------------------------------------------*/
SCICOS_IMPEXP int sci_var2vec(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_vec2var(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_getblocklabel(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_scicos_debug(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_scicos_debug_count(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_sctree(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ftree2(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ftree3(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ftree4(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_curblock(char *fname,unsigned long fname_len);

SCICOS_IMPEXP int sci_scicos_time(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_duplicate(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_diffobjs(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_pointer_xproperty(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_phase_simulation(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_set_xproperty(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_set_blockerror(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ctree2(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ctree3(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_ctree4(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_scicosim(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_getscicosvars(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_curblockc(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_buildouttb(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_end_scicosim(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_haltscicos(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_coserror(char *fname, unsigned long fname_len);

/* it was in a separate gateway */
SCICOS_IMPEXP int sci_data2sig(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_sig2data(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_model2blk(char *fname,unsigned long fname_len);
SCICOS_IMPEXP int sci_callblk(char *fname,unsigned long fname_len);

SCICOS_IMPEXP int sci_loadScicos(char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_SCICOS__ */
/*--------------------------------------------------------------------------*/
