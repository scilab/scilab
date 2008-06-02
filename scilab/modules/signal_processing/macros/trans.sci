function [hzt]=trans(pd,zd,gd,tr_type,frq)
//hzt=trans(pd,zd,gd,tr_type,frq)
//macro for transforming standardized low-pass filter into
//one of the following filters:
//     low-pass, high-pass, band-pass, stop-band.
// hz      :input polynomial
// tr_type :type of transformation
// frq     :frequency values
// hzt     :output polynomial
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// Copyright (C) INRIA - 1998 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
  if typeof(pd)=="rational" then
    hz=pd;
    tr_type=zd
    frq=gd
    pd=roots(hz.den)
    zd=roots(hz.num)
    gd=coeff(hz.num,degree(hz.num))/coeff(hz.den,degree(hz.den))
  end
  z=poly(0,'z');fc=.25;fu=frq(1);fl=frq(2);
  //make filter type using all-pass change of variables
  select tr_type
  case 'lp' then
    alpha=sin(%pi*(fc-fu))/sin(%pi*(fc+fu));
    num=z-alpha;
    den=1-alpha*z;
  case 'hp' then
    alpha=-cos(%pi*(fc-fu))/cos(%pi*(fc+fu));
    num=-(1+alpha*z);
    den=z+alpha;
  case 'bp' then
    k=tan(%pi*fc)/tan(%pi*(fu-fl));
    alpha=cos(%pi*(fu+fl))/cos(%pi*(fu-fl));
    num=-((k+1)-2*alpha*k*z+(k-1)*z^2);
    den=(k+1)*z^2-2*alpha*k*z+(k-1);
  case 'sb' then
    k=tan(%pi*fc)*tan(%pi*(fu-fl));
    alpha=cos(%pi*(fu+fl))/cos(%pi*(fu-fl));
    num=(k+1)-2*alpha*z+(1-k)*z^2;
    den=(k+1)*z^2-2*alpha*z+(1-k);
  else
    error(msprintf(gettext("%s: Wrong value for input argument #%d: %s\n"),'trans',4,gettext('Unknown filter type.')));
  end
  [pt,zt,gt]=bilt(pd,zd,gd,num,den);
  hzt=rlist(gt*real(poly(zt,'z')),real(poly(pt,'z')),'d');
endfunction
