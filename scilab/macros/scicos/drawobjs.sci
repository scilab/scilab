function drawobjs(scs_m)
// Copyright INRIA
if type(scs_m)<>15 then return,end
nx=size(scs_m)
for i=2:nx
  drawobj(scs_m(i))
end
drawtitle(scs_m(1))
show_info(scs_m(1)(10))







