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
phi1=atan(-imag(z(:,1)),-real(z(:,1)))
[m,n]=size(z)
z=z(:,2:n)./z(:,1:n-1)
phi=[phi1 atan(imag(z),real(z))]
if n>1 then
  for l=1:m
    kk=find(abs(phi(l,2:n)-phi(l,1:n-1))>=2*%pi)
    if kk<>[] then phi(l,kk+ones(kk))=phi(l,kk),end
  end
end

phi=cumsum(phi,2)*(180/%pi)-180

//reset modulo 360
if part(mod,1)=='c' then
  mphi=maxi(phi)
  if mphi<=0 then 
    k=int(abs(mphi)/360)+1
    phi=phi+ones(phi)*360*k
  else
    k=int(mphi/360)+1
    phi=phi-ones(phi)*360*k
  end
else
  for l=1:m
    k=find(phi(l,:)>0)
    phi(l,:)=phi(l,:)-int(phi(l,:)/360)*360
    if k<>[] then phi(l,k)=phi(l,k)-360*ones(k),end
  end
end




