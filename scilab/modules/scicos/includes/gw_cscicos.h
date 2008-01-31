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
#ifndef __GW_CSCICOS__
#define __GW_CSCICOS__
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_cscicos)(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int sci_scicos_time _PARAMS((char *fname,unsigned long fname_len));
int sci_duplicate _PARAMS((char *fname,unsigned long fname_len));
int sci_diffobjs _PARAMS((char *fname,unsigned long fname_len));
int sci_pointer_xproperty _PARAMS((char *fname,unsigned long fname_len));
int sci_phase_simulation _PARAMS((char *fname,unsigned long fname_len));
int sci_set_xproperty _PARAMS((char *fname,unsigned long fname_len));
int sci_set_blockerror _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree2 _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree3 _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree4 _PARAMS((char *fname,unsigned long fname_len));
int sci_scicosim _PARAMS((char *fname,unsigned long fname_len));
int sci_getscicosvars _PARAMS((char *fname,unsigned long fname_len));
int sci_curblockc _PARAMS((char *fname,unsigned long fname_len));
int sci_buildouttb _PARAMS((char *fname,unsigned long fname_len));
int sci_permutobj _PARAMS((char *fname,unsigned long fname_len));
int sci_xstringb2 _PARAMS((char *fname,unsigned long fname_len));
int sci_end_scicosim _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CSCICOS__ */
/*--------------------------------------------------------------------------*/
