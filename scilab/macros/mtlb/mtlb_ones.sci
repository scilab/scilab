function r=mtlb_ones(a)
// Copyright INRIA
if size(a)==[1 1] then
  r=ones(a,a)
else
  r=ones(a(1),a(2))
end
