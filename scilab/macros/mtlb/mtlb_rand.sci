function r=mtlb_rand(a,opt)
// Copyright INRIA
[lhs,rhs]=argn()
if rhs==1 then
  if and(size(a)==[1 1]) then
    r=rand(a,a)
  else
    r=rand(a(1),a(2))
  end
else
  if and(size(a)==[1 1]) then
    r=rand(a,a,opt)
  else
    r=rand(a(1),a(2),opt)
  end
end
