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

function [%pt, scs_m] = do_tild(%pt, scs_m)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//** 16/07/2009, S. Steer, make it work with multiple selection
//** Alan 02/12/06 : use of objects swap in gh_curwin.children.children()
//**
//**  8 Aug 2007   : extended version  
//**
//**  August 2007 : first version with connected blocks
//** 
//*
//**  BEWARE: this version has some residual graphical bug with some 
//**          Modelica electrical connected blocks
//* 

    ks=Select(find(Select(:,2)==%win),1) //look for selected blocks in the current window
    if ks==[] then
       ks = getblock(scs_m, %pt(:))
    end
    

    if ks==[] then return;end
    scs_m_save = scs_m ; //** save the diagram for "Undo" ...

    for k=ks'
     
      //**--------- scs_m object manipulation -------------------
  
      path = list('objs',k)      ; //** acquire the index in the "global" diagram
  
      //** I need to flip the old object before the new one - in order to export 
      //** the right I/O ports  geometric coordinates - because the new one
      //** hinerit its geometric proprieties 
      o = scs_m.objs(k)    ; //** the old object 
      if and(typeof(o)<>["Link","Text"]) then
	o_geom = o.graphics  ; //** isolate the geometric proprieties 
	o_geom.flip = ~o_geom.flip ; //** flip the object 
	o.graphics = o_geom; //** update the old object
	scs_m.objs(k) = o ;  //** update the diagram 
       
	o_n  = o ; //** "o" is already flipped  
    
	scs_m = changeports(scs_m, path, o_n); 
      end
    end
  
    [scs_m_save, enable_undo, edited] = resume(scs_m_save, %t, %t)
endfunction
