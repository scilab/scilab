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

function drawobjs(scs_m, gh_window)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


   drawlater() ;
   
   //** to be moved to upper level 
   //** mark_size = int(%zoom*3.0); //** in pixel : size of the selection square markers

   rhs = argn(2) ; //** get the number of right side arguments
   
   if rhs==1 then //** without arguments (default) assume ...

       gh_curwin = gcf(); //** get the handle of the current graphics window

    else //** the argument is explicit
      
       gh_curwin = scf(gh_window) ; //** set the handle of the current graphics window

   end

   gh_axes = gca(); //** axes


   //** ** TEXT use integer size / non aliased fonts ***
   gh_axes.fractional_font = "off";
  
   //** ** ARC/CIRCLE use line/segment ***
   gh_axes.arc_drawing_method = "lines"; 
   
   //** draw grid     
   if exists('%scicos_with_grid') & %scicos_with_grid then
     drawgrid(gh_curwin);
   end

   //** Main Loop for objects 
   for i=1 : lstsize(scs_m.objs) //** draw object by object
      scs_m_index = i ; //** creation of a semiglobal variable for object indexing
      gh_blk = drawobj(scs_m.objs(i), gh_curwin);
   end

   //** Update selected object
   //** if the Select is NOT empty and some object are selected in the current window
   if (Select<>[]) & (find(Select(:,2)==gh_curwin.figure_id))<>[] then
     o_size = size (gh_axes.children);
     for i=find(Select(:,2)==gh_curwin.figure_id)
       gh_k = get_gri(Select(i,1),o_size(1)) ;
       gh_axes.children(gh_k).children(1).mark_mode = "on" ; //** set the mark
     end
   end

   drawtitle(scs_m.props)     ; //** 
   show_info(scs_m.props.doc) ; //** User defined function (usually empty, do nothing) 
   
   drawnow();
  
endfunction
