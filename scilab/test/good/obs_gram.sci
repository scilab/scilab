function [go]=obs_gram(a,c,domaine)
// Copyright INRIA
[lhs,rhs]=argn(0)
select type(a)
case 1  then
  if rhs<2 then error(39); end;
  if rhs==2 then domaine='c'; end;
  if part(domaine,1)<>'c' then domaine='d',end
  [m,n]=size(a)
  if m<>n then error(20,1),end
  [mb,nb]=size(c);if nb<>n then error(60),end
case 16 then
  flag=a(1)
  if flag(1)=='r' then a=tf2ss(a),end
  if flag(1)<>'lss' then error(91,1),end
  [a,c,domaine]=a([2,4,7])
  if domaine==[] then
    write(%io(2),'Warning: obs_gram--> By default time-domain = continuous')
    domaine='c';
  end
  [n,n]=size(a)
else 
  error('(A,C) pair or syslin list')
end;
//
s=spec(a)
if part(domaine,1)=='c' then
  if maxi(real(s))>=0 then error('Unstable system'),end
else
  if maxi(abs(s))>=1 then error('Unstable system'),end
end
go=lyap(a,-c'*c,domaine)
endfunction
