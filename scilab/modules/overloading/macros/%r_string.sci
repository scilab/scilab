// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function txt = %r_string(r)
    // string output of any array of rationals, of any number of dimensions (even>2)
    //
    N = string(r.num(:))
    D = string(r.den(:))
    nr = size(r,"*")
    i = 1:2:2*nr
    lengthI = max([length(N(i)) length(D(i))],"c")
    hruleMax = part("-",ones(1:max(lengthI)))
    hrules = strncpy(emptystr(nr,1)+hruleMax,lengthI)
    txt = emptystr(nr*5,1);
    j = 1:5:5*nr
    txt(j)   = N(i)    // Exponents of numerator
    txt(j+1) = N(i+1)  // Coefficients of numerator
    txt(j+2) = hrules  // -------------------------
    txt(j+3) = D(i)    // Exponents of denominator
    txt(j+4) = D(i+1)  // Coefficients of denominator

    // Centering the shortest part num or den on the other:
    // justify(,"c") can't be used on exponents rows due to their leading blanks
    txt = matrix(txt,5,-1)
    [lmin, irmin] = min(length(txt),"r")
    [lmax, irmax] = max(length(txt),"r")
    leftpaddlength = floor((lmax-lmin)/2)
    hpaddMax = part(" ", ones(1:max(leftpaddlength)))
    padding = strncpy(emptystr(1,nr)+hpaddMax, leftpaddlength)
    k = irmin<3  //  columns are %T where the num is the shortest
    txt(1,k)  = padding(k)  + txt(1,k)
    txt(2,k)  = padding(k)  + txt(2,k)
    txt(4,~k) = padding(~k) + txt(4,~k)
    txt(5,~k) = padding(~k) + txt(5,~k)

    // We reshape the result
    s = size(r)
    s(1) = -1
    txt = matrix(txt,s)
endfunction
