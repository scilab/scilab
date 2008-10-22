//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function Fitdiagramtofigure_()
//** Alan-21/12/06 : Fit diagram to figure (Based on new graphics)
   Cmenu = [];
   xinfo("Fit diagram to figure");
   gh_curwin = scf(%win) ; //** get the handle of the current graphics window
   gh_axes = gca(); 

   r = gh_curwin.figure_size;     //** acquire the current figure physical size
   rect = dig_bound(scs_m);       //** Scicos diagram size
   if rect==[] then               //** if the schematics is not defined
     return;                      //**   then return
   end
   w = (rect(3)-rect(1));
   h = (rect(4)-rect(2));
   margins = [0.02 0.02 0.02 0.02]
   %zoom_w=r(1)/(w*(1+margins(1)+margins(2)))
   %zoom_h=r(2)/(h*(1+margins(3)+margins(4)))

   //** to debug
   if %scicos_debug_gr then
     printf("zoom=%f\n",%zoom);
     printf("zoom_w=%f\n",%zoom_w);
     printf("zoom_h=%f\n",%zoom_h);
   end

   %zoom = min(%zoom_w,%zoom_h);

   gh_window = gcf();             //* get handle of current window

   window_set_size(gh_window);

   if %scicos_with_grid then
     drawgrid();
     swap_handles(gh_axes.children($),...
                  gh_axes.children(1));
     delete(gh_axes.children(1));
   end
   drawnow();
   //** show_pixmap() ; //** not useful on Scilab 5
   xinfo(' ');
endfunction
