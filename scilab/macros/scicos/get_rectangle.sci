function [ox,oy,w,h,ok]=get_rectangle(xc,yc)
// Copyright INRIA
pat=xget('pattern')
xset('pattern',default_color(0))
dr=driver()
if dr=='Rec' then driver('X11'),end
//
rep(3)=-1
ox=xc
oy=yc
w=0;h=0
ok=%t
first=%t
pixmap=xget('pixmap')==1

while rep(3)==-1 do
  xrect(ox,oy,w,h)
  if pixmap then xset('wshow'),end  
  if first then 
    rep=xgetmouse();
  else 
    rep=xgetmouse(0),
  end
  if rep(3)==-100 then //active window has been closed
    driver(dr);
    ok=%f
    return
  end
  xrect(ox,oy,w,h)
  if pixmap then xset('wshow'),end  
  xc1=rep(1);yc1=rep(2)
  ox=mini(xc,xc1)
  oy=maxi(yc,yc1)
  w=abs(xc-xc1);h=abs(yc-yc1)
  first=%f
end
if or(rep(3)==[2 5]) then ok=%f,end

driver(dr)
xset('pattern',pat)
endfunction
