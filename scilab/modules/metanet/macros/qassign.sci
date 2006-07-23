function [crit,order]=qassign(c,f,d)
// Copyright INRIA
[lhs,rhs]=argn(0)
if (rhs <> 3) then error(39), end
n=size(c,1);n1=size(c,2);
if(n<>n1)  then
  error('Bad dimensions of input arrays')
end;
n=size(f,1);n1=size(f,2);
if(n<>n1)  then
  error('Bad dimensions of input arrays')
end;
df=sum(diag(f));
if(df<>0)  then
  error('Diagonal of second input array must be zero')
end;
dd=sum(diag(d));
if(dd<>0)  then
  error('Diagonal of third input must be zero')
end;
n=size(d,1);n1=size(d,2);
if(n<>n1)  then
  error('Bad dimensions of input arrays')
end;
n3=n+n+n;n4=n3+n;
[a,b]=m6permuto(n,n3,n4,c,f,d)
crit=a;
order=b(1:n);
endfunction
