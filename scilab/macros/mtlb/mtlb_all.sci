function r=mtlb_all(a,dim)
// Copyright INRIA
// Emulation function for all() Matlab function
// V.C.
rhs=argn(2)

if type(a)==4 then a=bool2s(a);end

// B=all(A)
if rhs==1 then
  if ~isreal(a) then
    a=abs(a)
  end
  if size(a,1)==1|size(a,2)==1 then
    r=and(a)
  elseif size(a,1)==0|size(a,2)==0 then
    r=and(a)
  else
    r=and(a,1)
  end
// B=all(A,dim)
else
  // Because Matlab tolerate dim to be an non-existant dimension of a
  if dim>size(size(a),"*") then
    if isempty(a) then
      r=[]
    else
      r=a<>0
    end
  else
    if ~isreal(a) then
      a=abs(a)
    end
    r=and(a,dim);
  end
end
endfunction
