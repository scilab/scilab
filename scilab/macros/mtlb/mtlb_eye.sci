function r=mtlb_eye(a)
// Copyright INRIA
if size(a)==[1 1] then
  r=eye(a,a)
else
  r=eye(a(1),a(2))
end
