// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nk,W]=hankelsv(sl,tol)
//!
  if typeof(sl)<>'state-space' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: State-space linear system expected.\n"),"hankelsv",1)),
  end
  if sl.dt==[] then
    warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"hankelsv",1));
    sl.dt='c'
  elseif sl.dt<>'c' then 
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Continuous-time linear system expected.\n"),"hankelsv",1)),
  end
  //
  [lhs,rhs]=argn(0),
  if rhs==1 then tol=1000*%eps,end,
  lf=spec(sl(2)),
  if mini(abs(lf))<=tol then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Pure imaginary poles unexpected.\n"),"hankelsv",1)),
  end
  if maxi(real(lf)) > tol then 
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"hankelsv",1)),
  end,
  [sla,sls,d]=dtsi(sl);
  lc=ctr_gram(sls),lo=obs_gram(sls),W=lc*lo;
  nk=gsort(real(spec(W)));
endfunction
