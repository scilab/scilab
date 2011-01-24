//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function XcosMenuSaveAsPalette()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// Copyright INRIA

    Cmenu = [] ;
    spmode = pal_mode
    pal_mode = %t
    ierr=execstr('blk_tmp=PAL_f(''define'')','errcatch')
    if ierr<>0 then
      messagebox('Block palette not found.','modal')
      clear numk
      return
    end
    blk_tmp.graphics.sz=20*blk_tmp.graphics.sz
    scs_m=do_purge(scs_m)
    blk_tmp.model.rpar=scs_m
    scs_m_tmp=scicos_diagram(version=get_scicos_version())
    scs_m_tmp.objs(1) = blk_tmp
    scs_m=scs_m_tmp
    clear scs_m_tmp
    clear blk_tmp
    [scs_m,editedx] = do_SaveAs()
    scs_m=scs_m.objs(1).model.rpar
    //if TCL_EvalStr('set toto [winfo exists .palettes]')=='1' then
    //  PalTree_
    //end
    if ~super_block then edited=editedx,end
    pal_mode=spmode
endfunction

