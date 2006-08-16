function [sm,cwp]=cspect(nlags,ntp,wtype,x,y,wpar)
//<sm,cwp>=cspect(nlags,ntp,wtype,x,y,wpar)
//Spectral estimation using the modified periodogram method.
//Cross-spectral estimate of x and y is calculated when both
//x and y are given.  Auto-spectral estimate of x is calculated
//if y is not given.
//  x        :data if vector, amount of input data if scalar
//  y        :data if vector, amount of input data if scalar
//  nlags    :number of correlation lags (pos. integer)
//  ntp      :number of transform points (pos. integer)
//  wtype    :window type ('re','tr','hm','hn','kr','ch')
//  wpar     :optional window parameters
//           :         for wtype='kr', wpar>0
//           :         for wtype='ch', 0<wpar(1)<.5, wpar(2)>0
//  sm       :power spectral estimate in the interval [0,1]
//  cwp      :calculated value of unspecified Chebyshev
//           :window parameter
//
//!
//author: C. Bunks  date: 16 Sept 1988
// Copyright INRIA

   [lhs,rhs]=argn(0);
   cross=0;
 
//construct window
 
   if rhs==4 then,
      w=window(wtype,2*nlags-1);
   else if rhs==5 then,
      if wtype=='kr' then,
         wpar=y;
         w=window(wtype,2*nlags-1,wpar);
      else if wtype=='ch' then,
         wpar=y;
         [w,cwp]=window(wtype,2*nlags-1,wpar);
      else,
         cross=1;
         w=window(wtype,2*nlags-1);
      end,
      end,
   else,
      [w,cwp]=window(wtype,2*nlags-1,wpar);
      cross=1;
   end,
   end,
 
//estimate correlations
 
   if maxi(size(x))==1 then,
      nsects=int(x/(3*nlags));
      xlen=int(x/nsects);
      ss=0*ones(1,2*nlags);
      if cross==1 then,
         for k=1:nsects,
            xk=getx(xlen,1+(k-1)*xlen);
            yk=gety(xlen,1+(k-1)*xlen);
            ss=corr('update',xk,yk,ss);
         end,
         re=fft(ss,1)/x;
         re=[re(nlags:-1:1) re(2:nlags)];
      else,
         for k=1:nsects,
            xk=getx(xlen,1+(k-1)*xlen);
            ss=corr('update',xk,ss);
         end,
         re=fft(ss,1)/x;
         re=[re(nlags:-1:1) re(2:nlags)];
      end,
   else,
      if cross==1 then,
         [re,me]=corr(x,y,nlags);
         re=[re(nlags:-1:1) re(2:nlags)];
      else,
         [re,me]=corr(x,nlags);
         re=[re(nlags:-1:1) re(2:nlags)];
      end,
   end,
 
//window correlation estimate
 
   wre=w.*re;
 
//fourier transform to obtain spectral estimate
 
   wree=[wre 0*ones(1,ntp-2*nlags+1)];
   sm=abs(fft(wree,-1));
endfunction
