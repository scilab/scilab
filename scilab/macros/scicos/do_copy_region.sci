function [scs_m,needcompile]=do_copy_region(scs_m,needcompile)
[btn,xc,yc,win,Cmenu]=cosclick()
if Cmenu<>[] then
  Cmenu=resume(Cmenu)
end
disablemenus()
[reg,rect]=get_region(xc,yc,win)
// Copyright INRIA
if rect==[] then enablemenus();return,end
modified=size(reg)>1


xinfo('Drag to destination position and click (left to fix, right to cancel)')
rep(3)=-1
yc=yc-rect(4)  
dr=driver()
if dr=='Rec' then driver('X11'),end
pat=xget('pattern')
xset('pattern',default_color(0))
while rep(3)==-1 , //move loop
  // draw block shape
  xrect(xc,yc+rect(4),rect(3),rect(4))
  if pixmap then xset('wshow'),end
  // get new position
  rep=xgetmouse()
  // erase block shape
  xrect(xc,yc+rect(4),rect(3),rect(4))
if pixmap then xset('wshow'),end
  xc=rep(1);yc=rep(2)
  xy=[xc,yc];
end

driver(dr);xset('pattern',pat)
if rep(3)==2 then enablemenus();return,end

scs_m_save=scs_m,nc_save=needcompile
n=size(scs_m)
for k=2:size(reg)
  o=reg(k)
  // translate blocks and update connection index 
  if o(1)=='Link' then
    o(2)=o(2)-rect(1)+xc
    o(3)=o(3)-rect(2)+yc
    [from,to]=o(8:9)
    o(8)(1)=o(8)(1)+n-1;
    o(9)(1)=o(9)(1)+n-1;
  else
    o(2)(1)(1)=o(2)(1)(1)-rect(1)+xc
    o(2)(1)(2)=o(2)(1)(2)-rect(2)+yc
    if o(1)=='Block' then
      for i=5:8
	k_conn=find(o(2)(i)>0)
	o(2)(i)(k_conn)=o(2)(i)(k_conn)+n-1
      end
    end
  end
  scs_m($+1)=o
  drawobj(o)
end
if modified then 
  needcompile=4,
  enablemenus()
  [scs_m_save,nc_save,enable_undo,edited]=resume(scs_m_save,nc_save,%t,%t)
end
enablemenus()
