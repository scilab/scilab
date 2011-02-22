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

function reg=get_inside(SelectRegion)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  [ox,oy,w,h,win]=SelectRegion(:)
  kc=find(win==windows(:,2))
  if kc==[] then
    messagebox('This window is not an active palette','modal')
    return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    scs_m=palettes(kpal)
  elseif win==curwin then //click dans la fenetre courante
    scs_m=scs_m
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  else
    messagebox('This window is not an active palette','modal')
    return
  end
  [keep,del]=get_blocks_in_rect(scs_m,ox,oy,w,h)
  
  [reg,DEL,DELL]=do_delete1(scs_m,del,%f)
  reg=do_purge(reg)
endfunction
