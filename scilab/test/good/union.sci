function [x,ka,kb]=union(a,b)
//Copyrigth INRIA
// returns the union of  unique components of  vector a and b
[lhs,rhs]=argn()
if lhs==1 then
  [x,k]=unique([a(:);b(:)])
  x=x';k=k'
else
  na=size(a,'*')
  kab=[1:na,na+(1:size(b,'*'))]
  [x,k]=unique([a(:);b(:)])
  x=x'
  kab=kab(k)
  ka=kab(kab<na+1)
  kb=kab(kab>na)
  if kb<>[] then kb=kb-na,end
end
endfunction
