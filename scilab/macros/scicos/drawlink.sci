function drawlink(o)
// Copyright INRIA
ct=o(7);c=ct(1)
pos=ct(6)
thick=xget('thickness')
d=xget('dashes')
xset('dashes',c)
xset('thickness',max(pos(1),1))
  
xpoly(o(2),o(3),'lines')
xset('dashes',d)
xset('thickness',thick)




