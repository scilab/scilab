function []=xbasr(win_num)
// This function is used to redraw the content of the graphic 
// window win_num.
//!
// Copyright Enpc
// Copyright INRIA 2007

[lhs,rhs]=argn(0);
if rhs==0,
  draw(gcf()) ;
else 
   draw(scf(win_num)) ;
end 
endfunction
