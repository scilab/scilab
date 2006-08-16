function hz=iir(n,ftype,fdesign,frq,delta)
// hz=iir(n,ftype,fdesign,frq,delta)
//macro which designs an iir digital filter
//using analog filter designs.
//the arguments of the filter are:
//  n        :filter order (pos. integer)
//  ftype    :specification of the filter type
//           :   ftype=('lp','hp','bp','sb')
//  fdesign  :specifiation of the analog filter design
//           :   fdesign=('butt','cheb1','cheb2','ellip')
//  frq      :2-vector of discrete cut-off frequencies
//           :(i.e., 0<frq<.5). For lp and hp filters only
//           :frq(1) is used.  For bp and sb filters frq(1)
//           :is the lower cut-off frequency and frq(2) is
//           :the upper cut-off frequency
//  delta    :2-vector of error values for cheb1, cheb2, and
//           :ellip filters where only delta(1) is used for
//           :cheb1 case, only delta(2) is used for cheb2 case,
//           :and delta(1) and delta(2) are both used for
//           :ellip case.
//           :          0<delta(1),delta(2)<1
//           :for cheb1 filters:  1-delta(1)<ripple<1 in passband
//           :for cheb2 filters:  0<ripple<delta(2)   in stopband
//           :for ellip filters:  1-delta(1)<ripple<1 in passband
//           :                    0<ripple<delta(2)   in stopband
//
//!
// author: C. Bunks  date: 9 Sept 1988
// Copyright INRIA

//select analog filter design for low-pass filter with fc=.25

if maxi(abs(frq))>0.5 then error('iir:frq components must be less than 0.5'),end
if delta(1)<0|delta(2)>1 then error('iir: delta components must be in [0 1]'),end

[hs,pc,zc,gc]=analpf(n,fdesign,delta,2);
//make digital low-pass filter from analog low-pass filter
z=poly(0,'z');[pd,zd,gd]=bilt(pc,zc,gc,2*(z-1),(z+1));
//do change of variables to obtain general digital filter
hz=trans(pd,zd,gd,ftype,frq);
endfunction
