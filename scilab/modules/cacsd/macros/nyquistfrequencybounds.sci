// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [fmin,fmax]=nyquistfrequencybounds(H,bounds)
    //find frequencies that reaches the nyquist bounds on real an imag part

    fname="nyquistFrequencyBounds" //for error messages

    if or(size(bounds)<>[2 2]) then
        error(msprintf(_("%s: Wrong size for argument #%d: (%d,%d) expected.\n"),fname,2,2,2))
    end
    if or(bounds(2,:)<bounds(1,:)) then
        error(msprintf(_("%s: Wrong value for input argument #%d: second row must be greater than first one.\n"),fname,2))
    end
    if and(typeof(H)<>["state-space" "rational"]) then
        error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),fname,1))
    end
    if size(H,"*")<>1 then
        error(msprintf(_("Wrong type for argument #%d: SISO expected.\n"),fname,1))
    end

    if typeof(H)=="state-space" then
        H=ss2tf(H)
    end

    dom=H.dt
    if dom==[]|dom=="d" then dom=1,end
    eps=1d-10

    frq=[];
    orient=[]
    if dom=="c" then
        c=2*%i*%pi
        function f=arg2freq(y)
            f=y/c;
        endfunction
        Hr=(H+horner(H,-%s))/2;
        Hi=(H-horner(H,-%s))/2;
    else
        c=2*%i*%pi*dom
        function f=arg2freq(y)
            f=log(y)/c;
        endfunction
        Hr=(H+horner(H,1/%z))/2;
        Hi=(H-horner(H,1/%z))/2;
    end

    // computing frequencies that make the nyquist locus to cross
    // the real part lower bound
    f=arg2freq(roots(numer(Hr-bounds(1,1))));
    f=real(f(real(f)>0&abs(imag(f)./abs(f))<eps));
    //keep only those that give imaginary parts inside the bounds
    im=imag(repfreq(H,f));
    f=f(im<=bounds(2,2)&im>=bounds(1,2))
    if f<>[] then
        orient=[orient imag(repfreq(derivat(Hr),f))<0]
        frq=[frq;f];
    end

    // computing frequencies that make the nyquist locus to cross
    // the real part upper bound
    f=arg2freq(roots(numer(Hr-bounds(2,1))));
    f=real(f(real(f)>0&abs(imag(f)./abs(f))<eps));
    //keep only those that give imaginary parts inside the bounds
    im=imag(repfreq(H,f));
    f=f(im<=bounds(2,2)&im>=bounds(1,2))
    if f<>[] then
        orient=[orient imag(repfreq(derivat(Hr),f))>0]
        frq=[frq;f];
    end

    // computing frequencies that make the nyquist locus to cross
    // the imaginary part lower bound
    f=arg2freq(roots(numer(Hi-%i*bounds(1,2))));
    f=real(f(real(f)>0&abs(imag(f)./abs(f))<eps));
    //keep only those that give real parts inside the bounds
    re=real(repfreq(H,f));
    f=f(re<=bounds(2,1)&re>=bounds(1,1))
    if f<>[] then
        orient=[orient real(repfreq(derivat(Hi),f))>0]
        frq=[frq;f];
    end

    // computing frequencies that make the nyquist locus to cross
    // the imaginary part upper bound
    f=arg2freq(roots(numer(Hi-%i*bounds(2,2))));
    f=real(f(real(f)>0&abs(imag(f)./abs(f))<eps));
    //keep only those that give real parts inside the bounds
    re=real(repfreq(H,f));
    f=f(re<=bounds(2,1)&re>=bounds(1,1))
    if f<>[] then
        orient=[orient real(repfreq(derivat(Hi),f))<0]
        frq=[frq;f];
    end

    if frq==[] then
        //check if there is at least one point inside the bounds
        r=repfreq(H,%pi)
        if real(r)<bounds(1,1)|real(r)>bounds(2,1)|imag(r)<bounds(1,2)| ...
            imag(r)>bounds(2,2) then
            fmin=[];fmax=[]
        else
            fmin=0;fmax=%inf
        end
    else
        //looking for the lowest frequency for which the locus enters the
        //area and the highest for which the locus leaves it.
        [frq,k]=gsort(frq,"g","i");
        orient=orient(k)
        if ~orient(1) then
            //locus starts inside the area
            frq=[0;frq]
        end
        if orient($) then
            //locus ends inside the area
            frq=[frq;%inf]
        end

        fmin=frq(1)
        fmax=frq($)

    end


endfunction

