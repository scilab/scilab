function [y,zf] = mtlb_filter(b,a,x,zi)
// Copyright INRIA
[lhs,rhs]=argn()
[m,n]=size(x)
x=matrix(x,1,m*n)
a=poly(a($:-1:1),'z','c')
b=poly(b($:-1:1),'z','c')
z=poly(0,'z')
da = degree(a)
db = degree(b)
if (da-db) > 0 then b=b*z^(da-db); end 
if (db-da) > 0 then a=a*z^(db-da); end
if  lhs==1 then
  if rhs==4 then
    y=rtitr(b,a,x,zi)
  else
    y=rtitr(b,a,x);
  end
else
  sl=tf2ss(rlist(b,a,[]))
  if rhs==4 then
    [y,zf]=flts(x,sl,zi)
  else
    [y,zf]=flts(x,sl)
  end
end
y=matrix(y,m,n)
endfunction
