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

function window_read_size(gh_window)
  
  r = gh_window.figure_size ; 
  gh_window.auto_resize = "off" ; //**

  gh_window.axes_size = scs_m.props.wpar(5:6)

  //** axes settings
  scf(gh_window) ; 
  gh_axes = gca(); 

  gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"

  gh_axes.margins = [0,0,0,0] ;     //**

  gh_axes.data_bounds = matrix(scs_m.props.wpar(1:4),2,2)

  wrect = [0 , 0, 1, 1] ;
  gh_axes.axes_bounds = wrect ;
  
  gh_window.viewport = [scs_m.props.wpar(7), scs_m.props.wpar(8)]
		
  show_window(); //** put the current window in foreground

endfunction
