function f=%c_e(i,j,f)
// f=%c_e(i,j,f) f(i,j) extraction 
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2 then
  if type(i)==4 then i=find(i),end
  f=j(i)
else
  if type(i)==4 then i=find(i),end
  if type(j)==4 then j=find(j),end
  f=f(i,j)
end



