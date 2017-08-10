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


function [ar]=armac(a,b,d,ny,nu,sig)
    // just build a tlist for storing armacx coefficients
    //   A(z^-1)y= B(z^-1)u + D(z^-1)sig*e(t)
    //   a=<Id,a1,..,a_r>; matrix (ny,r*ny)
    //   b=<b0,.....,b_s>; matrix (ny,(s+1)*nu)
    //   d=<Id,d1,..,d_p>; matrix (ny,p*ny);
    //   ny : dim of observation y
    //   nu : dim of control  u
    //   sig : standard deviation  (ny,ny);
    //
    //!

    [na,la]=size(a);
    if na<>ny then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: row dimension must be equal to %d.\n"),..
        "armac",1, ny));

    end
    [nb,lb]=size(b);
    if nb<>0 & nb<>ny then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: row dimension must be equal to %d.\n"),..
        "armac",2, ny));
    end;
    if lb<>0 & nu<>0 then
        if modulo(lb,nu)<>0 then
            error(msprintf(gettext("%s: Wrong size of input argument #%d: Number of columns are incompatible with %s.\n"),..
            "armac",2,"nu"));
        end;
    end
    [nd,ld]=size(d);
    if nd<>ny then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: row dimension must be equal to %d.\n"),..
        "armac",3, ny));

    end
    ar=tlist(["ar","a","b","d","ny","nu","sig"],a,b,d,ny,nu,sig);
endfunction
