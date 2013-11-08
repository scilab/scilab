// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) INRIA -  Author: Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [gm,fr] = g_margin(h)
    // Compute the gain margin of a SISO transfer function

    if argn(2) < 1 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"g_margin",1));
    end

    select typeof(h)
    case "rational" then
    case "state-space" then
        h=ss2tf(h)
    else
        error(97,1),
    end;
    if or(size(h)<>[1 1]) then
        error(msprintf(_("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"g_margin",1))
    end
    //
    epsr=1.e-7;//used for testing if complex numbers are real
    eps1=1.e-7;//used for testing if complex numbers have a modulus near 1
    epssing=1e-10; //used for testing if arguments are not singular points of h
    if h.dt=="c" then  //continuous time case
        // get s such as h(s)=h(-s) and s=iw
        s=%i*poly(0,"w");
        //compute h(s)-h(-s)=num/den
        num=imag(horner(h.num,s)*conj(horner(h.den,s)))
        den=real(horner(h.den,s)*conj(horner(h.den,s)))
        //necessary condition
        w=roots(num,"e");
        ws=real(w(abs(imag(w))<epsr&real(w)<=0)) //points where phase is -180°

        //remove nearly singular points
        ws(abs(horner(num,ws))>=epssing*abs(horner(den,ws)))=[]
        if ws==[] then gm=%inf,fr=[],return,end
        mingain=real(freq(h.num,h.den,%i*ws))
    else  //discrete time case
        if h.dt=="d" then dt=1,else dt=h.dt,end
        //get z such as h(z)=h(1/z) and z=e^(%i*w*dt)
        //form hh=h(z)-h(1/z)
        z=poly(0,varn(h.den));
        sm=simp_mode();simp_mode(%f);hh=h-horner(h,1/z);simp_mode(sm)
        //find the numerator roots
        z=roots(hh.num,"e");
        z(abs(abs(z)-1)>eps1)=[]// retain only roots with modulus equal to 1

        //remove nearly singular points
        z(abs(horner(hh.num,z))>=epssing*abs(horner(hh.den,z)))=[];

        w=log(z)/(%i*dt)
        ws=real(w(abs(imag(w))<epsr)) //points where phase is -180°
        if ws==[] then gm=%inf,fr=[],return,end
        mingain=real(horner(h,exp(%i*ws*dt)))
    end

    k=find(mingain<0)
    if k==[] then gm=%inf,fr=[],return,end
    mingain=abs(mingain(k));
    ws=abs(ws(k))// select positive frequency

    gm=-20*log(mingain)/log(10) //tranform into Db
    [gm,k]=min(gm);ws=ws(k);//select the minimum

    fr=ws/(2*%pi) //transform in Hz
endfunction
