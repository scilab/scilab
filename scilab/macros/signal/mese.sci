function [sm,fr]=mese(x,npts);
//<sm,fr]=mese(x [,npts]);
//Calculate the maximum entropy spectral estimate of x
//  x    :Input sampled data sequence
//  npts :Optional parameter giving number of points of fr and sm
//        (default is 256)
//  sm   :Samples of spectral estimate on the frequency grid fr
//  fr   :npts equally spaced frequency samples in [0,.5)
//!
// author: C. Bunks  date: 24 August 1988
// revised: 9 April 1991
// Copyright INRIA

//default evaluation
 
   [rhs,lhs]=argn(0);
   if lhs==1 then,
      npts=256;
   end,
 
//estimate autocorrelation function of x
 
   Nx=length(x);
   r=convol(x,x(Nx:-1:1))
   r=r(Nx:-1:1)/Nx;
 
//get solution to the Yule-Walker equations
 
   [ar,sigma2,rc]=lev(r);
 
//compute spectrum
 
   ak=[1;ar];
   [sf,fr]=frmag(ak,npts);
   sm=sigma2*ones(sf)./(sf.*conj(sf));
endfunction
