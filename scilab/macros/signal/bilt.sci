function [npl,nzr,ngn]=bilt(pl,zr,gn,num,den)
//[npl,nzr,ngn]=bilt(pl,zr,gn,num,den)
//macro for calculating the gain poles and zeros
//which result from a bilinear transform or from
//a biquadratic transform.  Used by the macros iir
//and trans
//Note: ***This macro is not intended for general use***
//  pl  :input poles
//  zr  :input zeros
//  gn  :input gain
//  num :numerator of transform
//  den :denominator of transform
//!
//author: C. Bunks  date: 5 May 1989
//Updated: 15 Sep 1997

// Copyright INRIA
n=coeff(num);
   d=coeff(den);
   order=maxi(size(n))-1;
   ms=maxi(size(pl));ns=maxi(size(zr));

   if order==1 then
      n0=n(1);n1=n(2);
      if prod(size(d))==1 then d=[d,0];end
      d0=d(1);d1=d(2);
      if zr == [] then      
         ngn=1/prod(n1*ones(pl)-d1*pl);
      else
         ngn=prod(n1*ones(zr)-d1*zr)/prod(n1*ones(pl)-d1*pl);
      end
      if ms<>ns then ngn=real(gn*d1**(ms-ns)*ngn);else ngn=real(gn*ngn);end
      nzr=-(n0*ones(zr)-d0*zr)./(n1*ones(zr)-d1*zr);
      npl=-(n0*ones(pl)-d0*pl)./(n1*ones(pl)-d1*pl);
      if ms>ns then
         nzr=[nzr';-(d0/d1)*ones(ms-ns,1)];
      elseif ms<ns then
         npl=[npl';-(d0/d1)*ones(ms-ns,1)];
      end
   elseif order==2 then
      n0=n(1);n1=n(2);n2=n(3);
      d0=d(1);d1=d(2);d2=d(3);
      a=n2*ones(zr)-d2*zr;
      b=n1*ones(zr)-d1*zr;
      c=n0*ones(zr)-d0*zr;
      gz=a;
      z1=-b./(2*a)+sqrt((b./(2*a))**2-c./a);
      z2=-b./(2*a)-sqrt((b./(2*a))**2-c./a);
      a=n2*ones(pl)-d2*pl;
      b=n1*ones(pl)-d1*pl;
      c=n0*ones(pl)-d0*pl;
      gp=a;
      p1=-b./(2*a)+sqrt((b./(2*a))**2-c./a);
      p2=-b./(2*a)-sqrt((b./(2*a))**2-c./a);
      gw=d2;
      w1=-d1./(2*d2)+sqrt((d1./(2*d2))**2-d0./d2);
      w2=-d1./(2*d2)-sqrt((d1./(2*d2))**2-d0./d2);
      ngn=gn*prod(gz)/prod(gp);
      nzr=[z1,z2];
      npl=[p1,p2];
      if ms>ns then
         nzr=[nzr';-(d0/d1)*ones(ms-ns,1)];
      elseif ms<ns then
         npl=[npl';-(d0/d1)*ones(ms-ns,1)];
      end
      ngn=real(ngn*(gw**(ms-ns)));
   else
      error('error bilt --- wrong order transform')
   end
endfunction
