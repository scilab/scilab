function [phi,db]=phasemag(z,mod)
//
// Copyright INRIA
mod_def='c'     //continuous representation
//mod_def='m'   //representation modulo 360 degrees
[lhs,rhs]=argn(0)
if lhs==2 then 
  db=20*log(abs(z))/log(10),
end
if rhs<>2 then 
  mod=mod_def
end
//compute first phase value in  (-pi, pi]
phi1=atan(imag(z(:,1)),real(z(:,1)))
//compute phase increments in (-pi, pi]
z=z(:,2:$)./z(:,1:$-1)
dphi=atan(imag(z),real(z))
phi=cumsum([phi1 dphi],2)

if part(mod,1)<>'c' then  // reset modulo 360
  phi=modulo(phi,2*%pi)
end
phi=phi*180/%pi //transform in degree
endfunction
