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

function DisableAllMenus()
  %ws = intersect(winsid(), [inactive_windows(2);curwin]')
  %men = menus(1);

  //** This additiona double protection assure the existance
  //** of the variable (not produce the first time in case of
  //** Scicos used in batch mode
  if ~exists("btn_n") then
       btn_n = []; 
  end
  if ~exists("win_n") then
       win_n = []; 
  end
 
  for %w = %ws
  
    //** this filter out the windows that have been intentionally closed
    //** ... for more details see macros/scicos_auto/scicos.sci
    //**         EnableAllMenus()
    //**           [btn_n, %pt_n, win_n, Cmenu_n] = cosclick() ; 
    //**         DisableAllMenus(); //** this function is called here 
    //** 
    if ~((btn_n==-1000) & (%w==win_n)) 
      for k=2:size(%men,'*')
       unsetmenu(%w,%men(k)); // Suppose here all windows have similar menus
      end  
    end   

  end
endfunction
