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

function [x]=nehari(r,tol)
    // [x]=nehari(R,tol) returns the Nehari approximant of R.
    // R = linear system in state-space representation (syslin list)
    //- R is strictly proper and - R~ is stable (i.e. R is antistable).
    //    || R - X ||oo = min || R - Y ||oo
    //                  Y in Hoo
    //!

    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),..
        "nehari",1))
    end

    if typeof(r)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"nehari",1))
    end
    if r.dt==[] then
        warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"nehari",1));
        r.dt="c"
    end
    if r.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"nehari",1))
    end
    //
    if argn(2)==1 then
        tol=1e-6
    else
        if type(tol)<>1|size(tol,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument: Scalar expected.\n"),"nehari",2))
        end
        if ~isreal(tol)|tol<=0 then
            error(msprintf(gettext( "%s: Input argument #%d must be strictly positive.\n"),"nehari",2))
        end
    end;

    //norm of Hankel operator
    //-----------------------
    nk=nophkel(r),nn=nk+tol,
    r.B=r.B/nn,
    //best approx.
    //------------
    xo=-obs_gram(r),xc=-ctr_gram(r),
    w=inv(eye()-xo*xc),
    [m,k,n]=size(r),m=m(1),
    [a,b,c]=abcd(r),o=0*ones(a),
    ax=[a,o,o;o,a,-w'*b*b';o,o,-a'-w*xo*b*b'],
    bx=[b;w'*b;w*xo*b],cx=[c,-c,0*ones(m,n)],
    x=syslin("c",ax,bx,cx*nn),
    [y,x]=dtsi(x);


endfunction

function [nk]=nophkel(sl,tol)
    //[nk]=nophkel(sl,[tol]) : norm of Hankel operator
    [lhs,rhs]=argn(0),
    if rhs==1 then tol=1000*%eps,end,
    if sl==0 then nk=0,return,end,
    lf=spec(sl.A),
    if min(abs(lf))<=tol then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Pure imaginary poles unexpected.\n"),"nehari",1))
    end,
    if max(real(lf))<tol then nk=0,return,end,
    sl=dtsi(sl);
    lc=ctr_gram(sl),lo=obs_gram(sl),
    vp=spec(lc*lo),vmax=max(real(vp)),
    nk=sqrt(vmax)
endfunction
