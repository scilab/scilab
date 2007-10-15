function [scs_m,needcompile] = do_replace(scs_m, needcompile, Clipboard, Select)
// Copyright INRIA

//**         BEWARE : This is an ---> EXPERIMENTAL VERSION <--- ! 
//**
//** This function is able to replace any block with any other block. 
//** Most of the works is done inside "changeports()".
  
  gh_curwin = scf(curwin) //** set the the current window and recover the handle
  
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
    
  scs_m = changeports(scs_m, path, o_n);
  
  //** upper level variable update  
  nc_save     = needcompile //** need explanation 
  needcompile = 4
  [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save, nc_save, %t, %t)

endfunction
