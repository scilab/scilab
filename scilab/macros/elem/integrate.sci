function x=integrate(expr,var,x0,x1,ea,er)
// x=integrate(expr,v,x0,x1 [,ea [,er]])  computes
//                      /x1
//                     [
//                 x = I  f(v)dv
//                     ]
//                    /x0
//
//
//examples:
//integrate('sin(x)','x',0,%pi)
//integrate(['if x==0 then 1,';
//           'else sin(x)/x,end'],'x',0,%pi)
//
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if size(x0,'*')<>1 then error('x0 must be a real scalar number'),end
if imag(x0)<>0 then error('x0 doit etre un scalaire reel'),end
[m,n]=size(x1),x1=matrix(x1,1,m*n)
if norm(imag(x1),1)<>0 then error('x1 is not real!'),end
//
deff('[ans]=func('+var+')',expr,'n')
x=[]
select rhs
case 4 then for xk=x1,x=[x,intg(x0,xk,func)],end,
case 5 then for xk=x1,x=[x,intg(x0,xk,func,ea)],end,
case 6 then for xk=x1,x=[x,intg(x0,xk,func,ea,er)],end,
else error(39), end,
x=matrix(x,m,n)
endfunction
