// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Francois DELBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [Ns,d]=coffg(Fs)
    // [Ns,d]=coffg(Fs) computes Fs^-1 where Fs is a polynomial
    // or rational matrix by co-factors method.
    // d = common denominator; Ns =  numerator (matrix polynomial)
    // Fs inverse = Ns/d.
    // (Be patient...results are generally reliable)
    // See also det, detr, invr, penlaur, glever, leverrier
    //!
    //
    if or(typeof(Fs)==["polynomial" "constant"]) then
        [n,np]=size(Fs);
        if n<>np then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"coffg",1))
        end
        d=det(Fs) // common denominator
        n1=n;
        for kk=1:n1,for l=1:n1,
                signe=(-1)^(kk+l);
                col=[1:kk-1,kk+1:n1];row=[1:l-1,l+1:n1];
                Ns(kk,l)=-signe*det(Fs(row,col))
        end;end
        Ns=-Ns;
    elseif typeof(Fs)=="rational" then
        [n,np]=size(Fs);
        if n<>np then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"coffg",1))
        end
        d=det(Fs) // common denominator
        n1=n;
        for kk=1:n1,for l=1:n1,
                signe=(-1)^(kk+l);
                col=[1:kk-1,kk+1:n1];row=[1:l-1,l+1:n1];
                Ns(kk,l)=-signe*det(Fs(row,col))
        end;end
        Ns=-Ns;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"detr",1))
    end
endfunction
