// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [y,R]=kpure(sl,eps)
  y=[];R=[];
  if argn(2)==1 then eps=[1e-6,1e-6],end
  select typeof(sl)
  case 'rational' then
    if size(sl.num,'*') > 1 then error(95,1),end
  case 'state-space' then
    sl=ss2tf(sl)
    if size(sl.num,'*') > 1 then error(95,1),end
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"kpure",1))
  end
  if sl.dt<>'c' then 
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"kpure",1))
  end
  if size(sl.num,'*')<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"kpure",1))
  end

  //build the Routh table of the given system
  r=routh_t(sl,poly(0,'k')),
  [s,t]=size(r);
  
  //Check of infinite solution
  for i=1:s,
    if and(coeff(r(i,:))==0) then 
      error(msprintf(gettext("%s: Infinite solution.\n"),"kpure")),
    end
  end,

  z=0;u=0;

  for i=1:s,
    k=roots(gcd(r(i,:)));// k is must be real
    k=real(k)
    //k(k<=0)=[]; //remove negative and zero values
    h=prod(size(k)),
    if h>0 then
      for j=1:h,
	if and(abs(k(j)-y)>eps(1)) then //remove duplicated k's
	  //compute the poles of the closed loop
	  wr=roots(k(j)*sl.num+sl.den) 
	  //retains k if at least one closed loop pole is imaginary	       
	  [w,ki]=mini(abs(real(wr)))
	  if w<eps(2) then 
	    y=[y real(k(j))],
	    R=[R wr(ki)]
	  end
	end
      end,
    end;
  end;

  [y,k]=gsort(y)
  R=R(k)
endfunction
