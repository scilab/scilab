function [str]=mtlb_dec2hex(D,N)
// Copyright INRIA
// Emulation function for Matlab dec2hex()
// V.C.

rhs=argn(2)

str=matrix(dec2hex(real(D)),-1,1)

if isempty(str) then
  str=""
end

if rhs==2 then
  for k=1:size(str,1)
    if str(k)<>"" then
      str(k)=part("0",ones(1,N-length(str(k))))+str(k)
    end
  end
end
endfunction


