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

function set_background(gh_window)

  
  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then //** without arguments (default) assume ... 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos]
     gh_curwin = gh_current_window ; //** get the handle of the current graphics window     
     
  else //** the arguments is explicit 
     gh_curwin = gh_window ; //** get the handle of the current graphics window
                             //** from the input argument of the function     
     
  end    
  
  gh_curwin.background = options.Background(1) ; //** "options" is sub structure of scs_m
  gh_curwin.children.background =  options.Background(1) ;
endfunction
