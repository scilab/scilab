function w=width2pixel(win,w)
// Copyright INRIA
  old=xget('window');xset('window',win);
  wp=xget('wpos');
  [x,y]=xchange([0,w],[0 0],'f2i');
  w=x(2)-x(1)
  xset('window',old);
endfunction
