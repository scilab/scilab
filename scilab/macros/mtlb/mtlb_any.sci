function r=mtlb_any(a,dim)
// Copyright INRIA
// Emulation function for any() Matlab function
// V.C.
rhs=argn(2)

if type(a)==4 then a=bool2s(a);end

// B=any(A)
if rhs==1 then
  if size(a,1)==1|size(a,2)==1 then
    r=or(a)
  elseif size(a,1)==0|size(a,2)==0 then
    r=or(a)
  else
    r=or(a,1)
  end
// B=any(A,dim)
else
  // Because Matlab tolerate dim to be an non-existant dimension of a
  if dim>size(size(a),"*") then
    if isempty(a) then
      r=[]
    else
      r=a<>0
    end
  else
    r=or(a,dim);
  end
end
endfunction
