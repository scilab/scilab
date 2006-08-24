function r=intc(a,b,f)
//If f is a complex-valued macro,intc(a,b,f) computes
//the integral from a to b of f(z)dz along the straight
//line a-b of the complex plane.
//!
//Author F.D.
//First compile f if necessary:
// Copyright INRIA
if type(f)==11 then comp(f),end;
//Define two functions which define the real part and
//imaginary part of f(g(t))*g'(t) where g(t) is a 
//parametrization of the line a-b.
deff('<r>=real1(t,a,b,f)','r=real(f((1-t)*a+t*b)*(b-a))')
deff('<r>=imag1(t,a,b,f)','r=imag(f((1-t)*a+t*b)*(b-a))')
r=intg(0,1,list(real1,a,b,f))+%i*intg(0,1,list(imag1,a,b,f))
endfunction
