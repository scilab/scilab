function [y,zf] = mtlb_filter(b,a,x,zi)
// Copyright INRIA
[lhs,rhs]=argn()
[m,n]=size(x)
x=matrix(x,1,m*n)
a=poly(a($:-1:1),'z','c')
b=poly(b($:-1:1),'z','c')
z=poly(0,'z')
b=b*z^(max(degree(a)-degree(b),0))
a=a*z^(max(degree(b)-degree(a),0))
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
