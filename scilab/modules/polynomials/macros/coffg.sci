// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Francois DELBECQUE
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [Ns,d] = coffg(Fs)
    // [Ns,d]=coffg(Fs) computes Fs^-1 where Fs is a polynomial
    // or rational matrix by co-factors method.
    // d = common denominator; Ns =  numerator (matrix polynomial)
    // Fs inverse = Ns/d.
    // (Be patient...results are generally reliable)
    // See also det, detr, invr, penlaur, glever, leverrier
    //!
    //
    if ~or(typeof(Fs)==["constant" "polynomial" "rational"]) then
        msg = gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n")
        error(msprintf(msg,"detr",1))
    end
    [n, np] = size(Fs);
    if n<>np then
        msg = gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n")
        error(msprintf(msg,"coffg",1))
    end

    if n==0 then
        Ns = [];
        d = [];
        return
    elseif n==1 then
        d = Fs;         // == det(Fs)
        Ns = 1 + 0/Fs;
        return
    end

    d = det(Fs) // common denominator
    for kk = 1:n
        for l = 1:n
            signe = (-1)^(kk+l);
            col = [1:kk-1, kk+1:n];
            row = [1:l-1, l+1:n];
            Ns(kk,l) = -signe*det(Fs(row,col));
        end
    end
    Ns = -Ns;
endfunction
