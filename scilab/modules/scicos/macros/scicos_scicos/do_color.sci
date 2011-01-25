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

function scs_m = do_color(%win, %pt, scs_m)
//
// do_color - edit a block / link color
//
//** 25/06/2009 : Serge Steer, 
//   -remove  objects which are not in the current window out of the selection
//   - add drawlater() for smooth drawing  
  if %win <> curwin then
     return ; //** exit point  
  end 
  
  if isempty(Select) then
    
     xc = %pt(1); yc = %pt(2);
    
     KK = getobj(scs_m, [xc;yc]);
     
     if isempty(KK) then
       return ; //** exit point (no object found) 
     end 
  
  else
    K=find(Select(:,2)==%win); //retains objects selected in the current window
    KK = Select(K,1)'; //** take all selected objects 

  end

  //**-----------------------------------------------------
  
  //** pick a color
  coul = getcolor('Choose a color:', 1); //** coul is the index of the chosen
                                         //** color in current figure's colormap
   
  //**----------------------------------------------------------
  //** the code below is modified according the new graphics API
  gh_curwin = scf(%win);
  gh_axes   = gca(); 

  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure 
  //** I need an equivalent index for the graphics 
  
  o_size = size(gh_axes.children); //** the size: number of all the object 
  
  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure 
  //** gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
  //** Alan Layec has done this in the function get_gri();
  //** gh_blk = gh_curwin.children.children(gh_k);
  drawlater()
  
  for K = KK                                  //** for all the selected object(s)
    o           = scs_m.objs(K);              //** the scs_m object  
     //** ------------------ Link --------------------------
    if typeof(o) == "Link" then
      [nam,pos,ct] = (o.id,o.thick,o.ct);
      c = coul ;
      if ~isempty(c) then
        connected = connected_links(scs_m,K);
        for kc = connected
          o = scs_m.objs(kc) ;
          ct = o.ct          ; //** <---- Haio :(
          if ct(1) <> c then
            o.ct(1) = c ;
	    gh_obj_K    = get_gri(kc, o_size(1));      //** compute the index in the graphics DS 
	    gh_compound = gh_axes.children(gh_obj_K); //** get the compound handle 
            gh_compound.children.foreground = c ;
            scs_m.objs(kc) = o ;
          end
        end
      end
    
    //** ------------------  Block -------------------------  
    elseif typeof(o) == "Block" then
      gh_obj_K    = get_gri(K, o_size(1));      //** compute the index in the graphics DS 
      gh_compound = gh_axes.children(gh_obj_K); //** get the compound handle 

      if type(o.graphics.gr_i)==10 then,
        o.graphics.gr_i = list(o.graphics.gr_i,[]),
      end
      
      if isempty(o.graphics.gr_i(2)) then
        coli = 0; 
      else
        coli = o.graphics.gr_i(2);
      end
      
      coln = coul;
      
      if ~isempty(coln) then
        if coln <> coli then
          o.graphics.gr_i(2) = coln ;   //** update the object propriety 
          scs_m.objs(K) = o ;           //** update the diagram 
          update_gr(gh_obj_K,o) ;       //** update the graphics data structure 
        end
      end
    
    //** ----------------- Text ---------------------------  
    elseif  typeof(o) == "Text" then
      //** not implemented yet
    end
  end

  drawnow(); //** force a redraw 

endfunction

