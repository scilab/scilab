// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -  Author: Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [gm,fr]=g_margin(h)
  //compute the gain margin of a SISO transfer function
  select typeof(h)
  case 'rational' then 
  case 'state-space' then 
    h=ss2tf(h)
  else 
    error(97,1),
  end;
  if or(size(h)<>[1 1]) then 
    error(msprintf(_("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"g_margin",1))
  end
  //
  eps=1.e-7;// threshold used for testing if complex numbers are real or pure imaginary
  if h.dt=='c' then  //continuous time case
    // get s such as h(s)=h(-s) and s=iw 
     s=%i*poly(0,"w");
     w=roots(imag(horner(h.num,s)*conj(horner(h.den,s))),"e")
     ws=real(w(abs(imag(w))<eps&real(w)<=0)) //points where phase is -180°
     ws(abs(horner(h.den,%i*ws))==0)=[];
     if ws==[] then gm=%inf,fr=[],return,end
     mingain=real(freq(h.num,h.den,%i*ws))
  else  //discrete time case
    if h.dt=='d' then dt=1,else dt=h.dt,end
    //get z such as h(z)=h(1/z) and z=e^(%i*w*dt)
    //form hh=h(z)-h(1/z)
    z=poly(0,varn(h.den));
    sm=simp_mode();simp_mode(%f);hh=h-horner(h,1/z);simp_mode(sm)
    //find the numerator roots
    z=roots(hh.num,"e");
    z(abs(abs(z)-1)>eps)=[]// retain only roots with modulus equal to 1
    w=log(z)/(%i*dt)
    ws=real(w(abs(imag(w))<eps)) //points where phase is -180°
    if ws==[] then gm=%inf,fr=[],return,end
    mingain=real(horner(h,exp(%i*ws*dt)))
  end
  k=find(mingain<0)
  if k==[] then gm=%inf,fr=[],return,end
  mingain=abs(mingain(k));
  ws=abs(ws(k))// select positive frequency
  //disp([ws,1/mingain])
  gm=-20*log(mingain)/log(10) //tranform into Db
  [gm,k]=min(gm);ws=ws(k)//select the minimum
  fr=ws/(2*%pi) 
endfunction
