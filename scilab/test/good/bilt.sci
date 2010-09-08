// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - C. Bunks
// Copyright (C) INRIA - 1997 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
  if type(pl)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n") ,"bilt",1))
  end
  if type(zr)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n") ,"bilt",2))
  end
  if type(gn)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n") ,"bilt",3))
  end
   if size(gn,'*')<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n") ,"bilt",3))
  end
  if type(num)<>2|size(num,'*')<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n") ,"bilt",4))
  end
  if type(den)<>2|size(den,'*')<>1 then 
     error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n") ,"bilt",5))
  end
  
  order=degree(den)
  
  if and(order<>[1 2]) then
    error(msprintf(gettext("%s: Wrong values for input argument #%d: degree must be in the set {%s}.\n"),"bilt",5,"1,2"))
  end
  if degree(num)<>order then
    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same degree expected.\n"),"bilt",4,5))
  end
  n=coeff(num);
  d=coeff(den);
  ms=max(size(pl));ns=max(size(zr));

  select order
    case 1  then
    n0=n(1);n1=n(2);
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
  case 2 then
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
  end
endfunction
