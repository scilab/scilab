// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
// Copyright (C) 2019 - Stéphane MOTTELET
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
    i = 1:nr
    lengthI = max([length(N(i)) length(D(i))],"c")
    hruleMax = part(ascii([226 148 128]),ones(1:max(lengthI)))
    hrules = strncpy(emptystr(nr,1)+hruleMax,lengthI)
    txt = emptystr(nr*3,1);
    j = 1:3:3*nr
    txt(j)   = N(i)    // numerator
    txt(j+1) = hrules  // ----------
    txt(j+2) = D(i)    // denominator

    // We reshape and justify the result
    s = size(r)
    s(1) = -1
    txt = matrix(justify(matrix(txt,3,-1),"c"),s)
endfunction
