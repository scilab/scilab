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

function K=lqg(P,r)
    // returns the (strictly proper) lqg (H2) controller
    // for the augmented plant P
    if and(typeof(P)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lqg",1))
    end

    if typeof(r)<>"constant"|~isreal(r) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"lqg",2))
    end
    if size(r,"*")<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"lqg",2,2))
    end
    r=int(r);
    if or(r<=0) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"lqg",2))
    end


    if typeof(P)=="rational" then
        P=tf2ss(P),flag=%f
    else
        flag=%t
    end

    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
    if norm(D11,1) <> 0 then
        warning(msprintf(gettext("%s: %s is not zero! (set to zero)"),"lqg","D11"));
    end
    //if norm(D22,1) <> 0 then warning('lqg: D22 is not zero!');end
    dom=P.dt;
    if dom==[] then
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"lqg",1));
        dom="c";
    end
    P12=syslin(dom,A,B2,C1,D12);
    Kc=lqr(P12);
    P21=syslin(dom,A,B1,C2,D21);
    Kf=lqe(P21);
    P22=syslin(dom,A,B2,C2,D22);
    K=obscont(P22,Kc,Kf);
    if ~flag then K=ss2tf(K);end
endfunction
