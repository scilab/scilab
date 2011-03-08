//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [ox,oy,w,h,ok] = get_rectangle(xc,yc)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


//** 18 Mar 2008 : update to "scicos_rubberbox()" for Scilab 5:
//**               the rubberduck solution :)

ox = xc
oy = yc
w=0 ; h=0
ok = %t
first = %t

[x_topleft, y_topleft] = (xc , yc )

rub_width  = 0 ;
rub_height = 0 ;

//** [x_left, y_top, width, height]

[final_rect, button] = scicos_rubberbox([xc; yc; rub_width; rub_height], %t) ;

if button == [2 5 12 -1000] then //** right button exit OR active window has been closed
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

  ox = min(xc, xc1) ; 
  oy = max(yc, yc1) ;
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
