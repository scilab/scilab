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

function drawgrid(gh_window)
//** get the number of right side arguments
   rhs = argn(2) ;

   if rhs==0 then //** without arguments (default) assume ...
       gh_curwin = gcf(); //** get the handle of the current graphics window
       gh_axes = gca(); 

    else
       gh_curwin = scf(gh_window) ; //** get the handle of the current graphics window
       gh_axes = gca(); 
   end

   if ~exists('%scs_wgrid') then
     %scs_wgrid = [10;10;12];
   end

   b1 = %scs_wgrid(1)  //** x
   b2 = %scs_wgrid(2); //** y
   colorr = %scs_wgrid(3) //**color

   bounds = gh_axes.data_bounds //**[xmin ymin;xmax ymax]
				
   


   // absolute coordinate of grid : always center in [0,0]
   //-Alan- : Please discuss if you want to change this behavior...
   loc_x = (int(bounds(1,1)/b1):1:int(bounds(2,1)/b1))*b1
   loc_y = (int(bounds(1,2)/b2):1:int(bounds(2,2)/b2))*b2
   
   xt=gh_axes.x_ticks
   xt.locations=loc_x;
   xt.labels=emptystr(loc_x)
   gh_axes.x_ticks=xt
   
   yt=gh_axes.y_ticks
   yt.locations=loc_y;
   yt.labels=emptystr(loc_y)
   gh_axes.y_ticks=yt
   
  
   if ~%scicos_with_grid(1) then
     gh_axes.grid=[-1 -1];
   else
      gh_axes.grid=[colorr colorr];
   end
endfunction

