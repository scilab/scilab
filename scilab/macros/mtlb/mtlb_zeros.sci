function r=mtlb_zeros(a)
// Copyright INRIA
if size(a)==[1 1] then
  r=zeros(a,a)
else
  r=zeros(a(1),a(2))
end
