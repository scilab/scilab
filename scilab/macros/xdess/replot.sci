function replot(rect)
// replots the graphic window win 
// using rect as x and y bounds 
// Copyright INRIA
win=xget("window");
xclear();xtape('replaysc',win,rect);
