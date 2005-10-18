function drawlink(o)
// Copyright INRIA
thick=xget('thickness');d=xget('dashes')

xset('dashes',o.ct(1));xset('thickness',max(o.thick(1),1))
  
xpoly(o.xx,o.yy,'lines')

xset('dashes',d);xset('thickness',thick)
endfunction
