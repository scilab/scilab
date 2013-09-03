// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [phm,fr]=p_margin(h)
    //compute the phase margin of a SISO transfer function
    select typeof(h)
    case "rational" then
    case "state-space" then
        h=ss2tf(h);
    else
        error(97,1)
    end
    if or(size(h)<>[1 1]) then
        error(msprintf(_("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"p_margin",1))
    end
    eps=1.e-7;// threshold used for testing if complex numbers are real or pure imaginary

    if h.dt=="c" then  //continuous time case
        w=poly(0,"w");
        niw=horner(h.num,%i*w);
        diw=horner(h.den,%i*w);
        // |n(iw)/d(iw)|=1 <-- (n(iw)*n(-iw))/(d(iw)*d(-iw))=1 <--  (n(iw)*n(-iw)) - (d(iw)*d(-iw))=0
        w=roots(real(niw*conj(niw)-diw*conj(diw)),"e");
        //select positive real roots
        ws=real(w(find((abs(imag(w))<eps)&(real(w)>0)))); //frequency points with unitary modulus
        if ws==[] then
            phm=[];
            fr=[];
            return
        end
        f=horner(h,%i*ws);
    else  //discrete time case
        if h.dt=="d" then
            dt=1;
        else
            dt=h.dt;
        end
        // |h(e^(i*w*dt))|=1 <-- h(e^(i*w*dt))*h(e^(-i*w*dt))
        z=poly(0,varn(h.den));
        sm=simp_mode();
        simp_mode(%f);
        hh=h*horner(h,1/z)-1;
        simp_mode(sm);
        //find the numerator roots
        z=roots(hh.num,"e");
        z(abs(abs(z)-1)>eps)=[];// retain only roots with modulus equal to 1
        w=log(z)/(%i*dt);
        ws=real(w(abs(imag(w))<eps&real(w)>0)); //frequency points with unitary modulus
        if ws==[] then
            phm=%inf;
            fr=[];
            return
        end
        f=horner(h,exp(%i*ws*dt));
    end
    phi=atand(imag(f),real(f));// phase of the frequency response (in [-180 180])

    //avoid near 0 negative phases that will give phm=180 instead of -180
    phi(phi>-1e-12&phi<0)=0;
    //compute the margins
    phm=pmodulo(phi,360)-180;
    //select the min value together with associated frequency in Hz
    [w,k]=min(abs(phm));
    phm=phm(k)
    fr=ws(k)/(2*%pi);
endfunction
