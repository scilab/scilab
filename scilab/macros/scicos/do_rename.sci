function [scs_m,edited]=do_rename(scs_m)
// Copyright INRIA
if pal_mode then
  mess='Enter the new palette name'
else
  mess='Enter the new diagram name'
end
new=dialog(mess,scs_m(1)(2)(1))
new=new(1)
if new<>[] then
  drawtitle(scs_m(1))  //erase title
  scs_m(1)(2)(1)=new,
  drawtitle(scs_m(1))  //draw title
  edited=%t
end
