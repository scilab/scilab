function [vd,D]=mtlb_eig(A,B)
// Copyright INRIA
// Emulation function for eig() Matlab function
// V.C.

[lhs,rhs]=argn()

if rhs==1 then
  if lhs==1 then
    vd=spec(A)
  else
    [vd,D]=spec(A)
    if isreal(vd,0) & size(vd,"*")>1 then
      vd(:,1:2)=-vd(:,1:2)
    end
  end  
else
  error("mtlb_eig() used with 2 rhs: not yet implemented !")
end
endfunction
