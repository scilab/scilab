function [rect,btn]=rubberbox(rect,edit_mode)
// Copyright INRIA, S. Steer
  rhs=argn(2)
  select rhs
  case 0 then
    edit_mode=%f
    initial_rect=%f
  case 1 then
    initial_rect=type(rect)==1 
    if ~initial_rect then  edit_mode=rect,end
  case 2 then
    initial_rect=%t
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
    rect(1)=xc;rect(2)=yc
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
  draw_rect(r,w,h)  //draw(r);if pix=='on' then show_pixmap();end
  while rep(3)==-1 do
    //** Alan, Ramine patch to disable rubberbox 12/10/07
    //if 1 then rep=xgetmouse(0,opt);else rep=xgetmouse(0,opt),end
    rep=xgetmouse(0,opt);
    if rep(3)==-100 then  btn=rep(3);return;end//window has been closed
    draw_rect(r,w,h) //draw(r);if pix=='on' then show_pixmap();end//erase;
    xc1=rep(1);yc1=rep(2)
    ox=mini(xc,xc1)
    oy=maxi(yc,yc1)
    w=abs(xc-xc1);h=abs(yc-yc1)
    r.data=[ox,oy,w,h]
     draw_rect(r,w,h)  //draw(r);if pix=='on' then show_pixmap();end //draw 
    first=%f
  end 
  draw_rect(r,w,h)  //draw(r);if pix=='on' then show_pixmap();end//erase
  delete(r)
  rect=[ox,oy,w,h]
  btn=rep(3)
  //f.pixmap=pix
  f.pixel_drawing_mode = pixel_drawing_mode
  f.immediate_drawing = immediate_drawing
endfunction

function draw_rect(r,w,h)
  // avoid refreshing the figure if it is not really needed
  // without this test the affich block's output is erased
  // often under Windows because a click by changing window
  // gives a pess_move
  if w+h>0.0001 then
    draw(r);if pix=='on' then show_pixmap();end //draw 
  end
endfunction
