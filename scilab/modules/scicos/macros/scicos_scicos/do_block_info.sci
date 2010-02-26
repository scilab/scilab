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

function %pt=do_block_info(%pt,scs_m)

  if Select==[] then
  //** if NO object is selected   
    win = %win;
    xc = %pt(1); yc = %pt(2); %pt = []
    
    //** look for block 
    kc = find(win==windows(:,2))
    if kc==[] then
      txt = "This window is not an active palette" 
      k = [];
      return
    
    elseif windows(kc,1)<0 then // click in a palette  
      kpal = -windows(kc,1)
      palette = palettes(kpal)
      k = getobj(palette,[xc;yc])
      if k<>[] then
          txt = get_block_info(palette,k)
      end
    
    elseif win==curwin then   // click in the current window 
      k = getobj(scs_m,[xc;yc])
      if k<>[] then
        txt = get_block_info(scs_m,k)
      end
    end
    
    if exists('txt') then
        messagebox(txt);
    end
    
  else
  
  //** Object selected  
    if size(Select,1)>1 then
      messagebox("Only one block can be selected for this operation.","modal")
      Cmenu=[]; %pt=[]; return
    end
    
    win = Select(1,2);
    kc = find(win==windows(:,2))
    k=Select(1,1)
    if kc==[] then
      txt="This window is not an active palette" ;
      k = [];
      return
    elseif windows(kc,1)<0 then // click in a palette  
      kpal = -windows(kc,1)
      palette = palettes(kpal)
      txt = get_block_info(palette,k)
    elseif win==curwin then // click in the current window 
      txt = get_block_info(scs_m,k),
    end
    messagebox(txt);
    
  end
  
endfunction
