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

function svm = svplot(Sl,w)
    //svplot singular-value sigma-plot.
    // svm = svplot(sl,w) computes for the linear dynamical system
    // sl, the singular values of its transfer function matrix:
    //                              -1
    //             g(jw) = c(jw*i-a)  b+d
    //
    //           or
    //                                        -1
    //             g(exp(jw)) = c(exp(jw)*i-a)  b+d
    //
    // evaluated over the frequency range specified by w.
    // sl is a sylin list (see syslin) representing the system
    // [a,b,c,d] in state-space form.
    // the i-th column of the output matrix svm contains the singular
    // values of g(exp(jw)) for the i-th frequency value.
    // svm = svplot(sl) is equivalent to
    // svm = svplot(sl,logspace(-3,3))  (continuous) or
    // svm = svplot(sl,logspace(-3,pi)) (discrete).
    //!

    [a,b,c,d]=abcd(Sl);
    // Reduce a to Hessenberg form
    [q,a] = hess(a); b = q'*b; c = c*q;
    // Compute the singular values of the frequency response
    select Sl.dt
    case []
        warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"svplot",1));
        if argn(2) == 1
            w = logspace(-3,3);
        end
        nf = max(size(w)); nsv = min(size(d)); j = sqrt(-1);
        svm(nsv,nf) = 0;
        for i = 1:nf
            svm(:,i) = svd(c*((j*w(i)*eye()-a)\b)+d);
        end
    case "c"
        if argn(2) == 1
            w = logspace(-3,3);
        end
        nf = max(size(w)); nsv = min(size(d)); j = sqrt(-1);
        svm(nsv,nf) = 0;
        for i = 1:nf
            svm(:,i) = svd(c*((j*w(i)*eye()-a)\b)+d);
        end
    case "d"
        if argn(2) == 1
            w = logspace(-3,%pi);
        end
        nf = max(size(w)); nsv = min(size(d)); j = sqrt(-1);
        svm(nsv,nf) = 0;
        for i = 1:nf
            svm(:,i) = svd(c*((exp(j*w(i))*eye()-a)\b)+d);
        end
    else T=Sl("dt");
        if argn(2) == 1
            w = logspace(-3,%pi);
        end
        nf = max(size(w)); nsv = min(size(d)); j = sqrt(-1);
        svm(nsv,nf) = 0;
        for i = 1:nf
            svm(:,i) = svd(c*((exp(j*w(i)*T)*eye()-a)\b)+d);
        end

    end
endfunction
