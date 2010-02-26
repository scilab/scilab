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

function pwindow_set_size(gh_window)
//  Set the physical figure size according to a given diagram
//  The "figure_size" controls ONLY the dimension of the physical window:
//        the dimension of the graphics canvas  is the "axes_size"
//        parameters ! 
// This function is called for creating the window when a diagram is opened.
  
  //** get the handle of the  graphics window     
  if argn(2)==0 then gh_window=gh_current_window;end
  gh_curwin = scf(gh_window) ; 
  gh_axes = gca();
  
  //** get the coordinate of the rectangle that include all the graphics object
  //  rect(1),rect(4)            rect(3),rect(4)   
  //	   +--------------------------+
  //       |                          |	   
  //	   |                          |	   
  //       +--------------------------+
  //  rect(1),rect(2)            rect(3),rect(2)
  rect = dig_bound(scs_m); 


  if rect<>[] then //non empty diagram
    %diag_size = [rect(3)-rect(1), rect(4)-rect(2)]; //** real size of the real diagram
    %wsiz = [max(400,%diag_size(1)), max(300, %diag_size(2))]; //** 400=minimum size for menus
  else // empty diagram (default values)
    %wsiz = [600/%zoom, 400/%zoom]; 
  end
 
  //** Scale the window to the correct zoom factor and limit the maximum size 
  %wdd = min(933, %zoom*%wsiz(1)) + 30 ;
  %hdd = min(700, %zoom*%wsiz(2)) + 30 ;
    
  //** set the the size of the physical window on the screen (in pixel)
  gh_curwin.figure_size = [ max(400,%wdd) max(300,%hdd) ] ; 

endfunction
