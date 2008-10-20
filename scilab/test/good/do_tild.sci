function [%pt,scs_m]=do_tild(%pt,scs_m)
// Copyright INRIA
//** 29 June 2006 : waiting for the random access to the graphics datastructure
//**                I force a Replot after the 
   
  win = %win;
  
  gh_curwin = gh_current_window ; //** acquire the current window handler 
  gh_curwin.pixmap ='on';         //** force pixmap mode
  
  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object

  xc = %pt(1) ; yc = %pt(2)   ;
  
  k = getblock(scs_m,[xc;yc]) ;
  
  if k==[] then return , end ; //** if you click in the void ... return back 

  if get_connected(scs_m,k)<>[] then //** see message 
     message('Connected block can''t be tilded')
     return
  end
  
  //**--------- scs_m object manipolation -------------------
  o = scs_m.objs(k) ; //** scs_m
  geom = o.graphics ; geom.flip = ~geom.flip ; o.graphics = geom;
  
  //**---------------------------------------------------------
  //gr_k = o_size(1) - k + 1 ; //** graphic 
  //
  //drawlater();
  //
  //  gh_blk = drawobj(o) ; //** draw the object "flipped" 
  //
  //  pause
  //   !--error 144 
  //     Undefined operation for the given operands
  //      check or define function %h_i_h for overloading
  ///     at line      37 of function do_tild called by :  
  //      line     6 of function Flip_ called by :  
  //      exec(Flip_,-1)
  //
  //  gh_curwin.children.children(gr_k) = gh_blk ; //** update the object in the old position
  //  
  //  delete(gh_blk) ; //** delete the previous object 
  //
  // drawnow(); show_pixmap() ;  
    
  //**------------------------------------------------------------
  
  scs_m_save = scs_m ; //** ... for undo ...
  
  scs_m.objs(k)=o

  [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)
  
  //** ... force a Replot() in the calling function 
  
endfunction
