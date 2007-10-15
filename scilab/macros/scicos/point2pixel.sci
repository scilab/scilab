function xyp=point2pixel(win,xy)
// Copyright INRIA
  old=xget('window');xset('window',win);
  wp=xget('wpos');
  [x,y,r]=xchange(xy(1),xy(2),'f2i');
  xyp=[wp(1)+x-r(1);wp(2)+y-r(2);]
  xset('window',old);
endfunction
