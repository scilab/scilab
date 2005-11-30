function select_unhilite(hilite_image)
//
// Copyright INRIA

n=length(hilite_image)
if   n==0 then return,end
  
win=hilite_image(1)(1)
if or(winsid()==win) then
  winback=xget('window')
  xset('window',win)
  alu=xget('alufunction')
  xset('alufunction',6)
else
  return
end


dr=driver()
if dr=='Rec' then driver('X11'),end
for i=1:n
  if hilite_image(i)(2)=='rect' then
    thickness= hilite_image(i)(5)
    orig=hilite_image(i)(3)
    sz=hilite_image(i)(4)
    thick=xget('thickness')
    
    xset('thickness',thickness);
    xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
    if pixmap then xset('wshow'),end
    xset('thickness',thick);
  elseif hilite_image(i)(2)=='poly' then
    thickness= hilite_image(i)(6)
    dashes= hilite_image(i)(5)
    xx=hilite_image(i)(3)
    yy=hilite_image(i)(4)
    thick=xget('thickness');d=xget('dashes')
    xset('thickness',thickness)
    xset('dashes',dashes)
    xpoly(xx,yy,'lines')
    xset('dashes',d)
    xset('thickness',thick)
    if pixmap then xset('wshow'),end
  end
end
driver(dr)  
xset('alufunction',alu)
xset('window',winback)
endfunction
