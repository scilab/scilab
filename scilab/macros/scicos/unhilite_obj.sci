function unhilite_obj(o)
//
// Copyright INRIA
dr=driver()
if dr=='Rec' then driver('X11'),end
if o(1)=='Block' then
  graphics=o(2);
  [orig,sz]=graphics(1:2)
  thick=xget('thickness')
  xset('thickness',6*thick);
  xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
  if pixmap then xset('wshow'),end  
  xset('thickness',thick);
elseif o(1)=='Link' then
  o(6)(1)=5*max(o(6)(1),1)
  drawobj(o)
  if pixmap then xset('wshow'),end
end
driver(dr)  
