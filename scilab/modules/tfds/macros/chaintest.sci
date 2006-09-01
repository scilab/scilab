function []=chaintest(f_l,b1,odem,xdim,npts)
//[]=chaintest([f_l,b1,odem,xdim,npts])
// Integration of the chain model
// a Three-species food chain model 
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs <=0, f_l=chain;end
if rhs <=1, b1=3.0;end
if rhs <=2, odem='default';end;
if rhs <=4, npts=[1000,0.1],end;
if rhs <=3 then 
   xdim=[0,1,0,1,5,11];
end;
x_message(["Integration of a Three-species food chain model "]);
portr3d(f_l,odem,xdim,npts);

endfunction
function [xdot]=chain(t,x)
fch_f1= ch_f1(x(1))
fch_f2= ch_f2(x(2))
x1= x(1)*(1-x(1)) - fch_f1*x(2)
x2= fch_f1*x(2) -  fch_f2*x(3) - 0.4*x(2)
x3= fch_f2*x(3) - 0.01*x(3)
xdot=[x1;x2;x3];

endfunction
function [z1]=ch_f1(u)
z1=5*u/(1+b1*u)

endfunction
function [z2]=ch_f2(u)
z2=0.1*u/(1+2*u)
endfunction
