function [x,k]=unique(x)
//Copyrigth INRIA
// extract unique components of a vector
if size(x,'*')==1 then
  k=1
else
  [lhs,rhs]=argn()
  if lhs==2 then
    [x,k]=sort(x);
    if type(x)==1 then x=x($:-1:1);k=k($:-1:1);end
    keq=find(x(2:$)==x(1:$-1))
    x(keq)=[]
    k(keq)=[]
  else
    x=sort(x);x=x($:-1:1);
    x(find(x(2:$)==x(1:$-1)))=[]
  end
end
