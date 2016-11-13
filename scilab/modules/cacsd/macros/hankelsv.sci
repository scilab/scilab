// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [nk,W]=hankelsv(sl,tol)
    //!
    if typeof(sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: State-space linear system expected.\n"),"hankelsv",1)),
    end
    if sl.dt==[] then
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"hankelsv",1));
        sl.dt="c"
    elseif sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Continuous-time linear system expected.\n"),"hankelsv",1)),
    end
    //
    [lhs,rhs]=argn(0),
    if rhs==1 then tol=1000*%eps,end,
    lf=spec(sl(2)),
    if min(abs(lf))<=tol then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Pure imaginary poles unexpected.\n"),"hankelsv",1)),
    end
    if max(real(lf)) > tol then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"hankelsv",1)),
    end,
    [sla,sls,d]=dtsi(sl);
    lc=ctr_gram(sls),lo=obs_gram(sls),W=lc*lo;
    nk=gsort(real(spec(W)));
endfunction
