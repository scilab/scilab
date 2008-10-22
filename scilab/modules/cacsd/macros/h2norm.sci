// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nh]=h2norm(g,tol)
//
//                 /+00
//     2           |          *
//  |g| =1/(2*%pi).|trace[g(jw).g(jw)]dw
//     2           |
//                 /-00

  if type(g)==1,if norm(g)==0,nh=0,return,end,end,
  [lhs,rhs]=argn(0),
  if rhs==1 then tol=1000*%eps,end;
  g1=g(1);
  select typeof(g)
  case 'state-space' then
    if norm(g(5))>0 then 
      error(msprintf(gettext("%s: Wrong value for input argument #%d: Proper system expected.\n"),"h2norm",1)),
    end;
    sp=spec(g(2)),
    if maxi(real(sp))>=-tol then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"h2norm",1)),
    end,
    w=obs_gram(g(2),g(4),'c'),
    nh=abs(sqrt(sum(diag(g(3)'*w*g(3))))),return,
  case 'rational' then
    num=g(2),den=g(3),
    s=poly(0,varn(den)),
    [t1,t2]=size(num),
    for i=1:t1,
      for j=1:t2,
	n=num(i,j),d=den(i,j),
	if coeff(n)==0 then 
	  nh(i,j)=0,
	else
	  if degree(n)>=degree(d) then
	    error(msprintf(gettext("%s: Wrong value for input argument #%d: Proper system expected.\n"),"h2norm",1)),
	  end
	  pol=roots(d),
	  if maxi(real(pol))>-tol then
	     error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"h2norm",1)),
	  end,
	  nt=horner(n,-s),dt=horner(d,-s),
	  nh(i,j)=residu(n*nt,d,dt),
	end,
      end
    end
    nh=sqrt(sum(nh)),return,
  else
    error(97,1)
  end
endfunction
