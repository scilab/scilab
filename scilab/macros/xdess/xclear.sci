function xclear(win_num)
//xbasc([win_num])
// Clear the graphic window win_num and erase the recorded graphics of
// window win_num
// if win_num is omited, it's the current graphic window
// win_num can also be a vector of window Id to clear a set of windows
//!
// Copyright ENPC

[lhs,rhs]=argn(0);
vvv=xget("old_style");

if vvv==0 then
    if rhs==0, win_num=xget("window"), win=gget('current_figure'),
       gset(win,'visible','off') ;end
    [n1,n2]=size(win_num);
    yyy=xget('window');
    for xxx=win_num, xset('window',xxx); win=gget('current_figure'),
       gset(win,'visible','off') ;end
    xset('window',yyy);
else
    oldxclear(win_num)
end
