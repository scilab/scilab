function [x,ka,kb]=intersect(a,b)
//Copyrigth INRIA
// returns the vector of common values of two vectors
[lhs,rhs]=argn()
if a==[]|b==[] then x=[],ka=[],kb=[],return,end
[a,ka]=unique(a(:));
[b,kb]=unique(b(:));
na=size(a,'*');
kab=[ka;kb];
[x,k]=sort([a;b]);x=x($:-1:1)';k=k($:-1:1);
kab=kab(k)';
keq=find(x(2:$)==x(1:$-1));
if keq==[] then
  x=[],ka=[];kb=[]
else
  x=x(keq)
  ka=kab(keq+1)
  kb=kab(keq)
end
endfunction
