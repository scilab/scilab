//  Scicos
//
//  Copyright (C) INRIA - Serge Steer <serge.steer@inria.fr>
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

function [rect,btn] = scicos_rubberbox(rect,edit_mode)

//** 18 Mar 2008 : this function has been renomed "scicos_rubberbox" in order to 
//**               avoid confusion with the Scilab 5 internal "C" primitive
//**               "rubberbox" 


  rhs = argn(2) ; 
  
  select rhs
    case 0 then
      edit_mode    = %f ;
      initial_rect = %f ;
    case 1 then
      initial_rect = type(rect)==1 ;
      if ~initial_rect then
        edit_mode = rect
      end
    case 2 then
      initial_rect = %t ; 
  end

  if edit_mode then 
    sel = 0:2 ; //** only button press requested
  else 
    sel = 0:5 ; //** press and click
  end

  opt = [%t edit_mode] ;

  if ~initial_rect
    while %t
      [btn,xc,yc] = xclick(0); //** BEWARE !!!
      if or(btn==sel) then 
         break ; //** EXIT point 
      end
    end
    rect(1) = xc;
    rect(2) = yc;
  end

  if size(rect,'*')==2 then
    rect(3)=0;
    rect(4)=0;
  end

  rep(3) = -1 ; 
  ox = rect(1); xc = ox ;
  oy = rect(2); yc = oy ;
  w = rect(3); h = rect(4)
  f = gcf();

  immediate_drawing = f.immediate_drawing;
  f.immediate_drawing = "off";
  pix = f.pixmap;
  
  pixel_drawing_mode = f.pixel_drawing_mode;
  //**  Scilab 5 specific mode 
  f.pixel_drawing_mode = "equiv"; //** BEWARE : "xor" mode !!!
  
  xrect(ox,oy,w,h) ;
  r = gce(); 
  r.foreground = -1 ;
  
  draw(r)

  //** ----- Scilab 5 compliant code inside  ---------
  while rep(3)==-1 do
      rep = xgetmouse(opt); //** update for Scilab 5
      
      //** window closing protection 
      if rep(3)==-1000 then // window has been closed
        btn = rep(3);
        return ; //** EXIT 
      end

      draw(r) ; //** delete visual 
  
      xc1 = rep(1) ;
      yc1 = rep(2) ; 
      ox  = mini(xc,xc1) ;
      oy  = maxi(yc,yc1) ; 
      w   = abs(xc-xc1) ;
      h   = abs(yc-yc1) ;
      r.data = [ox,oy,w,h] ;

      draw(r) ; //** update visual 

    end 
    
  delete(r)          ; //** delete selection box 
  rect = [ox,oy,w,h] ;
  btn = rep(3)       ;
  
  f.pixel_drawing_mode = pixel_drawing_mode ;
  f.immediate_drawing = immediate_drawing   ;

  drawnow(); //** update the visual 

endfunction
