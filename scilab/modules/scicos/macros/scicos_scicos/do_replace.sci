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

function [scs_m,needcompile] = do_replace(scs_m, needcompile, Clipboard, Select)

//**         BEWARE : This is an ---> EXPERIMENTAL VERSION <--- ! 
//**
//** This function is able to replace any block with any other block. 
//** Most of the works is done inside "changeports()".
  
  gh_curwin = scf(curwin); //** set the the current window and recover the handle
  
  //** SOURCE OBJECT Preparation 
  o_n = Clipboard ; //** "Clipboard arrives with the correct graphic object 
  
  //** DESTINATION OBJECT Preparation 
  k = Select(1,1)     ; //** the "Select" obj is the target 
  o = scs_m.objs(k) ;
    
  o_n.graphics.flip = o.graphics.flip // set same flip position
  
  
  [ox, oy] = getorigin(o)
      
  o_n.graphics.orig = [ox ,oy ] ; //** force the destination position 
   
  scs_m_save  = scs_m ; //** for "Undo" operation 
  
  path = list('objs',k)      ; //** acquire the index in the "global" diagram
    
  scs_m = changeports(scs_m, path, o_n); //** this is the routine that do the dirty job
  
  //** upper level variable update  
  nc_save     = needcompile //** need explanation 
  needcompile = 4
  [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save, nc_save, %t, %t)

endfunction
