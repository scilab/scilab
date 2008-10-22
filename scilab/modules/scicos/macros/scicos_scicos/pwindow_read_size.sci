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

function pwindow_read_size(gh_window)
  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then 
     gh_window = gh_current_window ;   
  end  
  gh_window.auto_resize = "on"  // due to scilab bug
  gh_window.figure_size = scs_m.props.wpar(9:10)
  gh_window.figure_position = scs_m.props.wpar(11:12)
endfunction
