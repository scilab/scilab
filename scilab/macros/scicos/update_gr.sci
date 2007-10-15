function update_gr(gr_k,o)
// Copyright INRIA
  gh_blk = drawobj(o) ; //** new object in gh_curwin.children.children(1)
  gh_blk.children(1).mark_mode = ...   //** for Select
      gh_curwin.children.children(gr_k+1).children(1).mark_mode;
  swap_handles(gh_curwin.children.children(gr_k+1),... //** update the object in the old position
               gh_curwin.children.children(1));
  delete(gh_curwin.children.children(1));
endfunction
