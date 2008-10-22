function x=%sp_l_sp(a,b)
// a\b , a sparse b sparse
// Copyright INRIA
[ma,na]=size(a)
[mb,nb]=size(b)
if ma<>mb then error(12),end
if ma<>na then
  b=a'*b;a=a'*a
end

[h,rk]=lufact(a)
if rk<mini(ma,na) then warning('deficient rank: rank = '+string(rk)),end
x=[]    
for k=1:nb
  x=[x,sparse(lusolve(h,full(b(:,k))))]
end
ludel(h)
endfunction
