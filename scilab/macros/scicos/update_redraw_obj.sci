function scs_m = update_redraw_obj(scs_m, path, o)
// Copyright INRIA
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()

if length(path)==2 then
  //** --------- LINK   and -------- TEXT --------------
  if typeof(o)=="Link" | typeof(o)=="Text" then
    //** Link and Text are update immediately here 
    scs_m(path) = o ; 
    //** quick update for new graphics
    drawlater() ;
     gh_curwin = gh_current_window;
     o_size = size(gh_curwin.children.children);
   //gr_k = o_size(1) - path(2) + 1; //** semi empirical equation :)
     gr_k = get_gri(path(2), o_size(1)) ; 
     update_gr(gr_k, o) //** update the graphics data structure only of the last object 
    draw(gh_curwin.children); //** update the graphics window 
    show_pixmap() ; //** show the diagram 
  else
    //** ---------------- BLOCK ------------------------
    //** the BLOCK and connected links are updated here 
    //** new graphics functions is now included in changeports
    scs_m = changeports(scs_m, path, o)
  end
  
else // change a block in a sub-level

  scs_m(path) = o ;

end

endfunction
