function x=%sp_inv(a)
// Copyright INRIA
[ma,na]=size(a)
if ma<>na then error(20,1),end
[hand,rk]=lufact(a)
if rk<na then ludel(hand);error(19),end
x=[]
for k=1:ma
  b=0*ones(ma,1);b(k)=1;
  x=[x,sparse(lusolve(hand,b))]
end
ludel(hand);
endfunction
