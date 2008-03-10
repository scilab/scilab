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
//!
// do_delete - delete a scicos object
// get first object to delete

  if %win<>curwin then
    disp("Window mismatch in do_delete"); 
    return ; //** EXIT point 
  end

  Select = Select(find(Select(:,2)==curwin),:)  //make sure only current window selects are considered

  win = %win;
  xc = %pt(1); yc = %pt(2)  ;
  K = getobj(scs_m,[xc;yc]) ;

  //** check the click over an empty object 
  if K==[]|intersect(K,Select(:,1)')<>[] then
        K=Select(:,1)'  // on empty space->delete selected blocks
        if K==[] then return,end  //nothing to do
  //otherwise consider only K
  end
  

  needreplay = replayifnecessary() ;
  scs_m_save = scs_m               ;
  nc_save = needcompile            ;

  [scs_m,DEL] = do_delete1(scs_m,K,%t); //** this is the function that do most of 
                                        //** the work and IS BUGGED ! (30/08/2007)

  //** 28 Aug 2006: Defused :)
  //** WARNING! : potentially dangerous code here : can ruin the coherence !
  //**            it not create problems because is impossible to manipoulate
  //*             deleted object (that results as negative gh_ datastructure 
  //**            index
  //** 28 Aug 2006: I fixed the potential problems using the standard mechanism
  //**              to update the graphics datastructure: now the coherency is
  //**              re-estabilished (La-Li-Lu-Le-Lo).

  //** gh_curwin = gh_current_window ; //** acquire the current window handler
  gh_curwin = scf(%win) ;
  gh_axes = gca(); 

  if DEL<>[] then //** if any object has been deleted .....

    needcompile = 4 ; //** signal to the compiler

    //** Update Selection
    if (Select<>[]) & (find(Select(:,2)==gh_curwin.figure_id))<>[] then
        new_Select = []
        for i=1:size(Select,1)
            if find(Select(i,1)==DEL)==[] | ...
             find(Select(i,2)==%win)==[] then
               new_Select = [new_Select; Select(i,:)];
            end
        end
        Select = new_Select;
    end

    //suppress right-most deleted elements
    //** while ("the last elements of "scs_m.objs" is 'Deleted' type ....
    while getfield(1,scs_m.objs($)) == "Deleted" then

      scs_m.objs($) = null(); //** erase the 'Deleted' elements from scs_m.objs
      gh_object_to_delete = gh_axes.children(1); //** the top element
      delete(gh_object_to_delete) ; //** delete the elements from the graphics datastructure 
                                    //** in order to mantain the coherency 
      if lstsize(scs_m.objs)==0 then
           break
      end

    end

    [scs_m_save,nc_save,enable_undo,edited,needreplay] = resume(scs_m_save,nc_save,%t,%t,needreplay);

  end


endfunction
