function [x]=sinc(n,fl)
//<x>=sinc(n,fl)
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
 



