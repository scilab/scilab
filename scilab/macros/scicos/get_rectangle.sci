function [ox,oy,w,h,ok] = get_rectangle(xc,yc)
// Copyright INRIA
//
//** using the new "rubberbox" function //** the rubberduck solution 

ox = xc
oy = yc
w=0 ; h=0
ok = %t
first = %t

[x_topleft, y_topleft] = (xc , yc )

rub_width  = 0 ;
rub_height = 0 ;

//** [x_left, y_top, width, height]

[final_rect, button] = rubberbox([xc; yc; rub_width; rub_height], %t) ;

if button == [2 5 12 -100] then // right button exit OR active window has been closed
    ok = %f ;
    return ; //** ---> Exit point 
end


//** final_rect: a rectangle defined by [x_left, y_top, width, height] 
x_left = final_rect(1) ; 
y_top  = final_rect(2) ; 
wid    = final_rect(3) ; 
hei    = final_rect(4) ; 


  xc1 = xc + wid ;
  yc1 = yc - hei ;

  ox = mini(xc, xc1) ; 
  oy = maxi(yc, yc1) ;
  w  = abs(xc - xc1) ;
  h  = abs(yc - yc1) ;
  ok = %t            ;

// disp("take a look here :) [get_rectangle]") ; pause
// 
// normal return parameters:  ox
//                            oy
//			      w
//			      h
//			      ok

endfunction
