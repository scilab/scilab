function hilite_obj(o)
//
// Copyright INRIA
dr=driver()
if dr=='Rec' then driver('X11'),end
if typeof(o)=='Block' then
  graphics=o.graphics;
  [orig,sz]=(graphics.orig,graphics.sz)
  thick=xget('thickness')
  xset('thickness',6*thick);
  xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
  if pixmap then xset('wshow'),end
  xset('thickness',thick);
elseif typeof(o)=='Link' then
  o.thick(1)=5*max(o.thick(1),1)
  drawobj(o)
  if pixmap then xset('wshow'),end
end
driver(dr)  
endfunction
