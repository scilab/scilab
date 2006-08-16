function x=ffilt(ft,n,fl,fh)
//x=ffilt(ft,n,fl,fh)
//Get n coefficients of a an FIR low-pass,
//high-pass, band-pass, or stop-band filter
//  ft :Filter type where ft can take the values
//     :       'lp' for low-pass filter
//     :       'hp' for high-pass filter
//     :       'bp' for band-pass filter
//     :       'sb' for stop-band filter
//  n  :Number of filter samples desired
//  fl :Low frequency cut-off
//  fh :High frequency cut-off
//     :For low and high-pass filters one cut-off
//     :frequency must be specified whose value is
//     :given in fl.  For band-pass and stop-band
//     :filters two cut-off frequencies must be
//     :specified for which the lower value is in
//     :fl and the higher value is in fh.
//  x  :Filter coefficients
//!
//author: C. Bunks  date: 12 March 1988
//        sinc replaced by filt_sync S Steer Jan 2002
// Copyright INRIA

//Pre-calculation
 
no2=(n-1)/2;
ino2=int(no2);
 
//Calculate n samples of the sinc function
 
//Low pass filter
 
if ft=='lp' then
  [x]=filt_sinc(n,fl)
end
 
//High pass filter
 
if ft=='hp' then
   x=filt_sinc(n,fl)
   x=-x;
   x(no2+1)=1+x(no2+1);
end
 
//Band pass filter
 
if ft=='bp' then
   wc=%pi*(fh+fl);
   fl=(fh-fl)/2;
   x=filt_sinc(n,fl)
   y=2*cos(wc*(-no2:no2));
   x=x.*y;
end
 
//Stop band filter
 
if ft=='sb' then
   wc=%pi*(fh+fl);
   fl=(fh-fl)/2;
   x=filt_sinc(n,fl)
   y=2*cos(wc*(-no2:no2));
   x=-x.*y;
   x(no2+1)=1+x(no2+1);
end
 
endfunction
function [x]=filt_sinc(n,fl)
//x=sinc(n,fl)
//Calculate n samples of the function sin(2*pi*fl*t)/(pi*t)
//for t=-n/2:n/2 (i.e. centered around the origin).
//  n  :Number of samples
//  fl :Cut-off freq. of assoc. low-pass filter in Hertz
//  x  :Samples of the sinc function
//!
// author: C. Bunks  date: 12 March 1988
// Copyright INRIA

   no2=(n-1)/2;
   ino2=int(no2);
   wl=fl*2*%pi;
   xn=sin(wl*(-no2:no2));
   xd=%pi*(-no2:no2);
   if ino2==no2 then xn(no2+1)=2*fl; xd(no2+1)=1;end
   x=xn./xd;
endfunction
