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

function [%pt,scs_m,needcompile,Select] = do_delete(%pt,scs_m,needcompile,Select)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// do_delete - delete a scicos object
  
// ** Serge Steer 24/06/2009
//  - make it more coherent with do_delete
//

// get first object to delete

  if %win<>curwin then
    messagebox("Window mismatch in do_delete","modal","error"); 
    return ; //** EXIT point 
  end

 
  Select1 = Select(find(Select(:,2)==curwin),:)  //** make sure only current window selects are considered
  if Select1==[] then
    xc = %pt(1); yc = %pt(2)  ;
    K = getobj(scs_m,[xc;yc]) ;
  else
    K=Select1(:,1)'
  end
  if K==[] then return,end  //nothing to do
  

  needreplay = replayifnecessary() ;
  scs_m_save = scs_m               ;
  nc_save = needcompile            ;

   //** eliminate the selected objects and connected links
  [scs_m,DEL] = do_delete1(scs_m,K,%t); 

  if DEL<>[] then //** if any object has been deleted .....

    needcompile = 4 ; //** signal to the compiler
    edited = %t
    enable_undo = %t

    //** Update Selection retaining selected objects in other windows
    //   (all selected objects of the current windows have been deleted)
    Select(find(Select(:,2)==curwin),:)=[];

    //**  suppress right-most "deleted" elements
    gh_curwin = scf(curwin);gh_axes = gca();  ; //** acquire the associated window axes handler
    while lstsize(scs_m.objs)>0&getfield(1,scs_m.objs($)) == "Deleted" then
      scs_m.objs($) = null(); //** erase the 'Deleted' elements from scs_m.objs
      delete(gh_axes.children(1)) ; //** delete the elements from the graphics datastructure 
    end

    [scs_m_save,nc_save,enable_undo,edited,needreplay] = resume(scs_m_save,nc_save,%t,%t,needreplay);

  end
endfunction
