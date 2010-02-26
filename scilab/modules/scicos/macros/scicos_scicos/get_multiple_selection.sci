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

function reg = get_multiple_selection(Select)
//Select contains the selected items in the window where the Copy menu
//has been executed
    
 win=Select(1,2)  // all items in the second column are identical

  kc=find(win==windows(:,2))

  if kc==[] then
    reg=[];return // window no longer active
  elseif windows(kc,1)<0 then //palette
    scs_m=palettes(-windows(kc,1))
  elseif win==curwin then //selected object in current window
    // scs_m is fine
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  end

  del=setdiff(1:size(scs_m.objs),Select(:,1))
  [reg,DEL,DELL]=do_delete1(scs_m,del,%f)
  reg=do_purge(reg)

endfunction


