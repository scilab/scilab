function [a,ka]=setdiff(a,b)
//Copyrigth INRIA
// returns a values which are not in b
na=size(a,'*');

[lhs,rhs]=argn()
[a,ka]=unique(a);
b=unique(b(:));

[x,k]=sort([a(:);b]);
if type(a)==1 then x=x($:-1:1);k=k($:-1:1);,end
d=find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
if d<>[] then k([d;d+1])=[],end

keep = find(k <= na); 
a = a(k(keep));
ka = ka(k(keep));
