// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//** 18 Feb 2008: Quick fix at the top by SM 

function [rect,btn] = rubberbox(rect, edit_mode)
//** initial_rect: vector with two or four entries.
//** With four entries it gives the initial rectangle defined by [x_left, y_top, width, height]
//** with two entries width and height are supposed to be 0,0.  

//** edition_mode:a boolean. The default value is "edition_mode=%f" (false)
//** if edition_mode==%f -> button press or click selects the first corner,
//** a click is requested to select the opposite corner.
//** if edition_mode==%t; button press selects the first corner, release
//** selects the opposite corner
  
  rhs = argn(2)
  select rhs
    case 0 then //** no arguments 
      edit_mode    = %f ; //** default 
      initial_rect = %f ; //** default 
    
    case 1 then //** only "rect" or "edit_mode" is given 
      initial_rect = type(rect)==1; //** the only arguments is really a rectangle ? 
                                    //** type 1 is a vector 
      if initial_rect then //** if the first arguments is positively a vector
         edit_mode = %f ;  //** assume the second parameter "edit_mode" as default 
      end
    
    case 2 then
      initial_rect = %t
  end

  if edit_mode then 
    sel=0:2,//only button press requested
  else 
    sel=0:5,//press and click
  end
  opt=[%t edit_mode]

  first=%t

  if ~initial_rect
    while %t
      [btn,xc,yc]=xclick(0)
      if or(btn==sel) then break,end
    end
    rect(1)=xc; rect(2)=yc
    //first=%f
  end
  if size(rect,'*')==2 then rect(3)=0;rect(4)=0,end
  rep(3)=-1
  ox=rect(1);xc=ox,
  oy=rect(2);yc=oy
  w=rect(3);h=rect(4)

  f=gcf();
  immediate_drawing=f.immediate_drawing;f.immediate_drawing='off';
  pix=f.pixmap;
  pixel_drawing_mode=f.pixel_drawing_mode;f.pixel_drawing_mode='xor';
  xrect(ox,oy,w,h)
  r=gce();r.foreground=-1;
  draw(r);if pix=='on' then show_pixmap();end
  while rep(3)==-1 do
    if first then rep=xgetmouse(opt);else rep=xgetmouse(0,opt),end
    if rep(3)==-100 then  btn=rep(3);return;end//window has been closed
    draw(r);if pix=='on' then show_pixmap();end//erase;
    xc1=rep(1);yc1=rep(2)
    ox=mini(xc,xc1)
    oy=maxi(yc,yc1)
    w=abs(xc-xc1);h=abs(yc-yc1)
    r.data=[ox,oy,w,h]
    draw(r);if pix=='on' then show_pixmap();end //draw 
    first=%f
  end 
  draw(r);if pix=='on' then show_pixmap();end//erase
  delete(r)
  rect=[ox,oy,w,h]
  btn=rep(3)
  //f.pixmap=pix
  f.pixel_drawing_mode = pixel_drawing_mode
  f.immediate_drawing = immediate_drawing
endfunction


