function xbasc(win_num)
//xbasc([win_num])
// Clear the graphic window win_num and erase the recorded graphics of
// window win_num
// if win_num is omited, it's the current graphic window
// win_num can also be a vector of window Id to clear a set of windows
//!
// Copyright ENPC
[lhs,rhs]=argn(0);
if rhs==0,win_num=xget("window");end
[n1,n2]=size(win_num);
for xxx=win_num,xclear(xxx);xtape('clear',xxx);end


