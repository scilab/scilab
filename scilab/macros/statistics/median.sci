function y=median(x,orient)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  n=size(x,'*')
  x=sort(x(:));x=x($:-1:1);
  if 2*int(n/2)==n then
    y = (x(n/2)+x(n/2+1))/2;
  else 
    y = x((n+1)/2);
  end
elseif orient=='r'|orient==1 then
  n=size(x,'r')
  x=sort(x,'r');x=x($:-1:1,:);
  if 2*int(n/2)==n then
    y = (x(n/2,:)+x(n/2+1,:))/2;
  else 
    y = x((n+1)/2,:);
  end
elseif orient=='c'|orient==2 then
  n=size(x,'c')
  x=sort(x,'c');x=x(:,$:-1:1);
  if 2*int(n/2)==n then
    y = (x(:,n/2)+x(:,n/2+1))/2;
  else 
    y = x(:,(n+1)/2);
  end
else
  error('median : second argument must be ''r'', ''c'', 1 or 2')
end
  
