// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - 2016 - INRIA -  Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [gm,fr] = g_margin(h)
// Compute the gain margin of a SISO transfer function

  if argn(2) < 1 then
    error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"g_margin",1));
  end
  if and(typeof(h)<>["state-space","rational"]) then
    error(msprintf(gettext("%s: Wrong type for input argument: Linear dynamical system expected.\n"),"g_margin",1))
  end
  if size(h,"*")<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"g_margin",1))
  end

  if typeof(h)=="state-space" then
    h=ss2tf(h)
  end


    ieee_save=ieee();ieee(2);
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
        ws=[];
        if w<>[] then
          ws=real(w(abs(imag(w))<epsr&real(w)<=0)) //points where phase is -180°
        end
        if ws<>[] then
          //remove nearly singular points
          ws(abs(horner(num,ws))>=epssing*abs(horner(den,ws)))=[]
        end
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
        if z<>[] then
          z(abs(abs(z)-1)>eps1)=[]// retain only roots with modulus equal to 1
        end
        if z<>[] then
          //remove nearly singular points
          z(abs(horner(hh.num,z))>=epssing*abs(horner(hh.den,z)))=[];
        end
        ws=[];
        if z<>[] then
          w=log(z)/(%i*dt)
          ws=real(w(abs(imag(w))<epsr)) //points where phase is -180°
        end
        if ws==[] then gm=%inf,fr=[],return,end
        mingain=real(horner(h,exp(%i*ws*dt)))
        ieee(ieee_save)
    end

    k=find(mingain<0)
    if k==[] then gm=%inf,fr=[],return,end
    mingain=abs(mingain(k));
    ws=abs(ws(k))// select positive frequency

    gm=-20*log(mingain)/log(10) //tranform into Db
    [gm,k]=min(gm);ws=ws(k);//select the minimum

    fr=ws/(2*%pi) //transform in Hz
    ieee(ieee_save)
endfunction
