function hilite_obj(o,win)
//
// Copyright INRIA
rhs=argn(2)
if rhs>1 then
  if or(winsid()==win) then
    winback=xget('window')
    xset('window',win)
    alu=xget('alufunction')
    xset('alufunction',6)
  else
    return
  end
end

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
if rhs>1 then 
  xset('alufunction',alu)
  xset('window',winback)
end

endfunction
