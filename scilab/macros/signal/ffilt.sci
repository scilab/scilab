function [x]=ffilt(ft,n,fl,fh)
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
// Copyright INRIA

//Pre-calculation
 
no2=(n-1)/2;
ino2=int(no2);
 
//Calculate n samples of the sinc function
 
//Low pass filter
 
if ft=='lp' then
  [x]=sinc(n,fl)
end
 
//High pass filter
 
if ft=='hp' then
   x=sinc(n,fl)
   x=-x;
   x(no2+1)=1+x(no2+1);
end
 
//Band pass filter
 
if ft=='bp' then
   wc=%pi*(fh+fl);
   fl=(fh-fl)/2;
   x=sinc(n,fl)
   y=2*cos(wc*(-no2:no2));
   x=x.*y;
end
 
//Stop band filter
 
if ft=='sb' then
   wc=%pi*(fh+fl);
   fl=(fh-fl)/2;
   x=sinc(n,fl)
   y=2*cos(wc*(-no2:no2));
   x=-x.*y;
   x(no2+1)=1+x(no2+1);
end
 



