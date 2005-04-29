function [rect,btn]=rubberbox(rect)
// Copyright INRIA

first=%t
if argn(2)<1 then
  rect=[]
  while %t
    [btn,xc,yc]=xclick(0)
    if or(btn==(0:5)) then break,end
  end
  rect(1)=xc;rect(2)=yc
  //first=%f
end
if size(rect,'*')==2 then rect(3)=0;rect(4)=0,end
//
rep(3)=-1
ox=rect(1);xc=ox,
oy=rect(2);yc=oy
w=rect(3);h=rect(4)

if get("figure_style")=="old" then
  pat=xget('pattern')
  xset('pattern',0)
  dr=driver()
  if dr=='Rec' then driver('X11'),end

  alu=xget('alufunction')
  xset('alufunction',6)
  while rep(3)==-1 do
    xrect(ox,oy,w,h)
    if xget('pixmap') then xset('wshow'),end  
    if first then rep=xgetmouse();else rep=xgetmouse(0),end
    if rep(3)==-100 then //window has been closed
      btn=rep(3)
      driver(dr)
      return
    end
    xrect(ox,oy,w,h)
    if xget('pixmap') then xset('wshow'),end  
    xc1=rep(1);yc1=rep(2)
    ox=mini(xc,xc1)
    oy=maxi(yc,yc1)
    w=abs(xc-xc1);h=abs(yc-yc1)
    first=%f
  end
  xset('alufunction',alu)
  driver(dr)
  xset('pattern',pat)
else
  f=gcf();pix=f.pixmap;f.pixmap='on'
  xrect(ox,oy,w,h)
  r=gce();r.foreground=-1;
  while rep(3)==-1 do
    if first then rep=xgetmouse();else rep=xgetmouse(0),end
    if rep(3)==-100 then //window has been closed
      btn=rep(3)
      return
    end
    show_pixmap()
    xc1=rep(1);yc1=rep(2)
    ox=mini(xc,xc1)
    oy=maxi(yc,yc1)
    w=abs(xc-xc1);h=abs(yc-yc1)
    r.data=[ox,oy,w,h]
    first=%f
  end 
  delete(r)
  show_pixmap()
end
rect=[ox,oy,w,h]
btn=rep(3)

endfunction

  
