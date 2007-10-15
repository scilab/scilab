function set_viewport(viewport)
// Copyright INRIA

    //** Beware ! : I'm forced to use old graphics instructions because there are not
    //**            direct equivalent inside the new graphics (24 may 2006)
    //**       

  x = viewport(1) ;
  y = viewport(2) ;
  //** This correction is really needed BUT uses old graphics primitives :(
  xset("viewport",x,y); //** Set the position of the panner.

endfunction

