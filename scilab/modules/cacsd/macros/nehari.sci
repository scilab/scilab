// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x]=nehari(r,tol)
// [x]=nehari(R,tol) returns the Nehari approximant of R.
// R = linear system in state-space representation (syslin list)
//- R is strictly proper and - R~ is stable (i.e. R is antistable).
//    || R - X ||oo = min || R - Y ||oo
//                  Y in Hoo
//!

  [lhs,rhs]=argn(0);
  if typeof(r)<>'state-space' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"nehari",1))
  end
  if r.dt==[] then
    warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"nehari",1));
    r.dt='c'
  end
  if r.dt<>'c' then 
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"nehari",1))
  end
  //
  if rhs==1 then tol=1e-6,end,
  //norm of Hankel operator
  //-----------------------
  nk=nophkel(r),nn=nk+tol,
  r(3)=r(3)/nn,
  //best approx.
  //------------
  xo=-obs_gram(r),xc=-ctr_gram(r),
  w=inv(eye()-xo*xc),
  [m,k,n]=size(r),m=m(1),
  [a,b,c]=r(2:4),o=0*ones(a),
  ax=[a,o,o;o,a,-w'*b*b';o,o,-a'-w*xo*b*b'],
  bx=[b;w'*b;w*xo*b],cx=[c,-c,0*ones(m,n)],
  x=syslin('c',ax,bx,cx*nn),
  [y,x]=dtsi(x);


endfunction

function [nk]=nophkel(sl,tol)
//[nk]=nophkel(sl,[tol]) : norm of Hankel operator
  [lhs,rhs]=argn(0),
  if rhs==1 then tol=1000*%eps,end,
  if sl==0 then nk=0,return,end,
  lf=spec(sl(2)),
  if mini(abs(lf))<=tol then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Pure imaginary poles unexpected.\n"),"nehari",1))
  end,
  if maxi(real(lf))<tol then nk=0,return,end,
  sl=dtsi(sl);
  lc=ctr_gram(sl),lo=obs_gram(sl),
  vp=spec(lc*lo),vmax=maxi(real(vp)),
  nk=sqrt(vmax)
endfunction
