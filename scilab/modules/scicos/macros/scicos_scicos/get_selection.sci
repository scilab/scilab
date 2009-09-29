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

function [win,o,%pt] = get_selection(Select)

  num = Select(1,1); win=Select(1,2)
  
  kc = find(win==windows(:,2))

  //**----------------------------------------------
  if kc==[] then
    
    o = []; return // window no longer active
  
  elseif windows(kc,1)<0 then //palette
    
    scs_m = palettes(-windows(kc,1))
   
  elseif win==curwin then //selected object in current window
    
    // scs_m is fine
  
  elseif slevel>1 then
    
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  
  end //**----------------------------------------
  
  if num>size(scs_m.objs) then
     o = []; return // no longer exists
  end
  
  o = scs_m.objs(num)
  
  if typeof(o)=="Block" then
    o = disconnect_ports(o)
  elseif typeof(o)=="Deleted" then  
    o = []  
  end
  
  if argn(1)==3 then
    if or(typeof(o)==["Block" "Text"]) then
      [orig,sz] = (o.graphics.orig,o.graphics.sz)
      %pt = orig(:)+sz(:)/2
    elseif typeof(o)=="Link" then  
      %pt = [(o.xx(1)+o.xx(2))/2,(o.yy(1)+o.yy(2))/2] // middle of first segment
    
    end
  end


endfunction

  
