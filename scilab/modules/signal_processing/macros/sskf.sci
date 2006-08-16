function [xe]=sskf(y,f,h,q,r,x0)
//<xe>=sskf(y,f,h,q,r,x0)
//steady-state kalman filter
// y   :data in form [y0,y1,...,yn], yk a column vector
// f   :system matrix dim(NxN)
// h   :observations matrix dim(NxM)
// q   :dynamics noise matrix dim(NxN)
// r   :observations noise matrix dim(MxM)
//
// xe  :estimated state
//!
// Copyright INRIA

//get steady-state Kalman gain
 
x=ricc(f',h'/r*h,q,'disc') // steady state err cov
 k=x*h'/(h*x*h'+r)

 // estimate state
 kfd=(eye(f)-k*h)*f;
 [xe]=ltitr(kfd,k,y,x0);
endfunction
