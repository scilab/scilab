function [hst]=fsfir(hd,flag)
//<hst>=fsfir(hd,flag)
//macro for the design of FIR, linear phase filters
//using the frequency sampling technique
//  hd   : vector of desired frequency response samples
//  flag : is equal to 1 or 2,
//         according to the choice of type 1 or type 2 design
//  hst  : vector giving the approximated continuous response
//         on the chosen grid of frequencies
//!
//author: G. Le Vey  Date: 1 Febr. 1989
// Copyright INRIA

   n1=prod(size(hd));//filter length
   if flag==1 then
   hst=hd(1)*dirich(n,0,flag);
   for j=1:n1-1,if hd(j+1)<>0,..
   hst=hst+hd(j+1)*(dirich(n,j,flag)+dirich(n,-j,flag));end;end
   else
   if n1==n/2 then
   hst=hd(1)*(dirich(n,.5,1)+dirich(n,-.5,1));
   for j=2:n1,if hd(j)<>0,..
   hst=hst+hd(j)*(dirich(n,j-1,flag)+dirich(n,-j+1,flag));end;end
   else
   hst=hd(n1)*dirich(n,0,1);
   hst=hst+hd(1)*(dirich(n,.5,1)+dirich(n,-.5,1));
   for j=2:n1-1,if hd(j)<>0,..
   hst=hst+hd(j)*(dirich(n,j-1,flag)+dirich(n,-j+1,flag));end;end
   end
   end

function [bidon]=dirich(n,p,q)
bidon=0.5;

