function x=%sp_l_s(a,b)
// a\b , a sparse b full
// Copyright INRIA

[ma,na]=size(a)
[mb,nb]=size(b)
if mb*nb==1 then x=a\(b*speye(na,na)),return;end

if ma<>mb then error(12),end

if ma<>na then b=a'*b;a=a'*a;end

if isreal(a)&isreal(b) then
  [h,rk]=lufact(a)
  if rk<mini(ma,na) then warning('deficient rank: rank = '+string(rk)),end
  x=[]    
  for k=1:nb
    x=[x,lusolve(h,b(:,k))]
  end
  ludel(h)
else
    [h,rk]=lufact([real(a) -imag(a);imag(a) real(a)])
  if rk<2*mini(ma,na) then warning('deficient rank: rank = '+string(rk/2)),end
  x=[]    
  for k=1:nb
    x=[x,lusolve(h,[real(b(:,k));imag(b(:,k))])]
  end
  x=x(1:$/2,:)+%i*x($/2+1:$,:)
  ludel(h)
end
endfunction
