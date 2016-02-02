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

function fr=freson(h)
    [lhs,rhs]=argn(0);
    [n,d]=h(["num","den"]);
    if type(n)==1 then
        n=poly(n,varn(d),"c");
    end
    if coeff(d,0)==0 then
        error(msprintf(_("%s: Wrong value for input argument #%d: infinite gain at zero frequency.\n"),"freson",1))
    end
    //look for  omega such that derivative of magn. is zero
    niw=horner(n,%i*poly(0,"w"));
    diw=horner(d,%i*poly(0,"w"));
    niw=real(niw*conj(niw));diw=real(diw*conj(diw));
    modul_d=derivat(niw/diw);w=roots(modul_d.num);

    // get extreme points
    k=find(imag(w)==0&real(w)>=0);
    if k==[] then
        fr=[];
        g=[];
        return
    end
    w=gsort(real(w(k)),"g","i");

    //find maximums
    wx=max(w)+0.5;
    if horner(modul_d,wx)<0 then
        w=w($:-2:1);
    else
        w=w($-1:-2:1);
    end
    fr=w/(2*%pi);
endfunction
