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

function [n]=linf(g,eps,tol)
    //linf(g,[eps],[tol])  L_infinity norm
    //    n=sup [sigmax(g(jw)] (sigmax largest singular value).
    //       w
    //-- g is a syslin system.
    //-- eps is error tolerance on n.
    //-- tol threshold for imaginary axis poles.
    // See also: h_norm
    //!
    if type(g)==1,if norm(g)==0,n=0,return,end,end,

    if and(typeof(g)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"linf",1))
    end
    if g.dt<>"c"&g.dt<>[] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"linf",1))
    end
    g.dt="c"
    if typeof(g)=="rational" then g=tf2ss(g),end

    [lhs,rhs]=argn(0),
    select rhs,
    case 1 then eps=1e-7,tol=1000*%eps,
    case 2 then tol=1000*%eps,
    end,
    [a,b,c,d]=g(2:5),[t,t]=size(a),
    p=ctr_gram(g),q=obs_gram(g);
    //Algorithm:
    //----------
    //1. min , max.
    //----------------------------------
    [slp,slm]=dtsi(g),
    if slp==0 then pp=0,qq=0,tp=1,
        pm=p,qm=q,tm=t,
    else
        if slm==0 then pm=0,qm=0,tm=1,
            pp=p,qq=q,tp=t,
        else
            [tp,tp]=size(slp(2)),[tm,tm]=size(slm(2)),
            pp=ctr_gram(slp),qq=obs_gram(slp),
            pm=ctr_gram(slm),qm=obs_gram(slm),
        end,
    end,
    hsvp=sqrt(spec(pp*qq)),hsvp=gsort(real(hsvp)),
    hsvm=sqrt(spec(pm*qm)),hsvm=gsort(real(hsvm)),
    gl=max([norm(d),hsvp(tp),hsvm(tm)]),
    gu=norm(d)+2*(sum(hsvp)+sum(hsvm)),
    //2. binary search
    //----------------------
    while gu-gl>2*eps*gl,
        x=(gl+gu)/2,
        r=d'*d-(x*x)*eye(),s=d*d'-(x*x)*eye(),
        mx=[a-b/r*d'*c, -x*b/r*b'; ..
        x*c'/s*c,   -a'+c'*d/r*b'],
        mp=abs(real(spec(mx))),mp=min(mp),
        if mp>tol then gu=x, else gl=x, end,
    end;
    n=(gu+gl)/2
endfunction
