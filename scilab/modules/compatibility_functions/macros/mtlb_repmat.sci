function repm=mtlb_repmat(M,m,n)
// Copyright INRIA
// Emulation function for repmat() Matlab function
// V.C.

str=%f
if type(M)==10 then
  str=%t
  M=asciimat(M)
end

if ~isreal(m) then
  m=real(m)
end

if or(type(M)==[4,6]) then
  M=bool2s(M)
end

rhs=argn(2)

// repmat(M,m) -> repmat(M,m,m)
if rhs==2 then
  n=m
end
// repmat(M,[m,n,p,...])
if rhs==2 & size(m,"*")<>1 then
  tmp=list()
  for k=1:size(m,"*")
    tmp(k)=m(k)
  end
  repm=ones(tmp(1:$)).*.M
// repmat(M,m,n)
else
  if ~isreal(n) then
    n=real(n)
  end
  if or(type(n)==[4,6]) then
    n=bool2s(n)
  end
  repm=ones(m,n).*.M
end
if str then
  repm=asciimat(repm)
end
endfunction

