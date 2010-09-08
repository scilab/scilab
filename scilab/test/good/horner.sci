// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function r=horner(p,x)
// horner(P,x) evaluates the polynomial or rational matrix P = P(s)
// when the variable s of the polynomial is replaced by x
// x can be a scalar or polynomial or rational matrix.
// Example: bilinear transform; Assume P = P(s) is a rational matrix
// then the rational matrix P((1+s)/(1-s)) is obtained by
// horner(P,(1+s)/(1-s));
// To evaluate a rational matrix at given frequencies use
// preferably the freq primitive ;
// See also: freq, repfreq.
//!
//
  if argn(2)<>2 then 
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),'horner',2))
  end
  if x==[]|p==[] then r=[],return,end
  tp=type(p)
  if tp<=2 then
    [m,n]=size(p)
    if m==-1 then indef=%t,m=1,n=1,p=p+0;else indef=%f,end
    if m*n==1 then //special case for a single polynomial
      C=coeff(p)
      r=C($)*ones(x)
      for kk=degree(p):-1:1,r=r.*x+C(kk);end;
    else
      r=[]
      for l=1:m
	rk=[]
	for k=1:n
	  plk=p(l,k)
	  d=degree(plk)
	  rlk=coeff(plk,d)*ones(x); // for the case horner(1,x)
	  for kk=1:d,
	    rlk=rlk.*x+coeff(plk,d-kk);
	  end;
	  rk=[rk rlk]
	end
	r=[r;rk]
      end
    end
    if indef then r=r*eye(),end
  elseif typeof(p)=='rational' then
    r=horner(p(2),x)./horner(p(3),x),
  elseif tp==129 then
    r=horner(p(:),x);r=r(1):r(2):r(3)
  else
    error(msprintf(gettext("%s: Unexpected type for input argument #%d.\n"),'horner',1))
  end
endfunction
