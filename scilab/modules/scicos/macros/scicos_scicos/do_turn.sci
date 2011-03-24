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

function [scs_m]=do_turn(%pt,scs_m,theta)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//** x,07/07 Alan : do_turn update theta value of blocks
//**                and text (redraw it)
//** 16/07/2009, S. Steer, make it work with multiple selection
  ks=Select(find(Select(:,2)==%win),1) //look for selected blocks in the current window
  if ks==[] then
    ks = getblock(scs_m, %pt(:))
  end
    

  if ks==[] then return;end
  scs_m_save = scs_m ; //** save the diagram for "Undo" ...

  for k=ks'
    path = list('objs',k) ; //** acquire the index in the "global" diagram
    o = scs_m.objs(k)
    if typeof(o)<>"Link" then

      //** Rotation enabled for "Block" and "Text"

      //**--------- scs_m theta update -------------------------
      geom = o.graphics ;
      geom.theta = geom.theta + theta ;
  
      //** angle normalization 
      while geom.theta>=360 then
	geom.theta = geom.theta-360;
      end
  
      while geom.theta<=-360 then
	geom.theta = geom.theta+360;
      end
  
      //** 
      o.graphics = geom ;

      scs_m.objs(k) = o ; 

      o_n  = o ; //** "o" is already rotated

      scs_m = changeports(scs_m, path, o_n); //** the real object draw is done here 
    end
  end
  
  [scs_m_save,enable_undo,edited] = resume(scs_m_save,%t,%t); 

endfunction
