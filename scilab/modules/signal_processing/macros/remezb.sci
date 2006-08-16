function an=remezb(nc,fg,ds,wt)
//an=remezb(nc,fg,ds,wt)
//Minimax approximation of a frequency domain
//magnitude response.  The approximation takes
//the form
//
//          h = sum[a(n)cos(wn)]
//
//for n=0,1,...,nc.  An FIR, linear-phase filter
//can be obtained from the the output of the macro
//by using the following Scilab commands
//
//          hn(1:nc-1)=an(nc:-1:2)/2;
//          hn(nc)=an(1);
//          hn(nc+1:2*nc-1)=an(2:nc)/2;
//
//  nc :Number of cosine functions
//  fg :Grid of frequency points in [0,.5)
//  ds :Desired magnitude on grid fg
//  wt :Weighting function on error on grid fg
//  an :Cosine filter coefficients
//!
// author: C. Bunks  date: 24 August 1988
// Copyright INRIA

//get frequency grid size
 
   ngrid=maxi(size(fg));
 
//check for errors in input
 
   f0=fg(1);
   f1=fg(ngrid);
   if f0<0 then
      error('values of fg must be in [0,.5] --- program stopped'),
   end,
   if f0>.5 then
      error('values of fg must be in [0,.5] --- program stopped'),
   end,
   if f1<0 then
      error('values of fg must be in [0,.5] --- program stopped'),
   end,
   if f1>.5 then
      error('values of fg must be in [0,.5] --- program stopped'),
   end,
   dsize=maxi(size(ds));
   wsize=maxi(size(wt));
   if dsize<>ngrid then
      error('fg and ds vectors are not the same length --- program stopped'),
   end,
   if wsize<>ngrid then
      error('fg and wt vectors are not the same length --- program stopped'),
   end,
 
//set up the initial guess for the extremal frequencies
 
   iext=round(1:ngrid/nc:ngrid);
   iext(nc+1)=ngrid;
   iext(nc+2)=ngrid;
 
//call remez.f
 
   an=remez(iext,ds,fg,wt);
   an=an(1:nc)
endfunction
