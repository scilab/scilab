function []=xbasr(win_num)
// This function is used to redraw the content of the graphic 
// window win_num. It works only with the driver "Rec"
//!
// Copyright Enpc
[lhs,rhs]=argn(0);
if rhs==0,
   win_num=xget("window");
   xclear(win_num);xtape('replay',win_num);
else 
   cw=xget("window");
   xset("window",win_num);xclear(win_num);xtape('replay',win_num);
   xset("window",cw);
end 




