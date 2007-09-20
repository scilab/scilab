function [%pt, scs_m] = do_tild(%pt, scs_m)
// Copyright INRIA
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

  //** dead code ? 
  //** win = %win ; //** acquire the window id (semiglobal)
  //** gh_curwin = gh_current_window ; //** acquire the current window handler
  //** o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object
  
  xc = %pt(1) ;
  yc = %pt(2) ;
  k = getblock(scs_m, [xc;yc]) ; //** try to recover the block over the "click" coordinate 

  if k<>[] then
  
    //** A valid block index is found 
    scs_m_save = scs_m ; //** save the diagram for "Undo" ...
    
    //**--------- scs_m object manipulation -------------------
  
    path = list('objs',k)      ; //** acquire the index in the "global" diagram
  
    //** I need to flip the old object before the new one - in order to export 
    //** the right I/O ports  geometric coordinates - because the new one
    //** hinerit its geometric proprieties 
    o = scs_m.objs(k)    ; //** the old object 
    o_geom = o.graphics  ; //** isolate the geometric proprieties 
    o_geom.flip = ~o_geom.flip ; //** flip the object 
    o.graphics = o_geom; //** update the old object
    scs_m.objs(k) = o ;  //** update the diagram 
       
    o_n  = o ; //** "o" is already flipped  
    
    scs_m = changeports(scs_m, path, o_n); 
  
    [scs_m_save, enable_undo, edited] = resume(scs_m_save, %t, %t)
  
  else 

    return ; //** EXIT: if you click in the void ... just return back
  
  end 
  
  
endfunction
