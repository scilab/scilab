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


function [A,B,D]=arma2p(ar)
    // Build three polynomial matrices
    // from an ar representation
    if ar(1)<>"ar" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s data structure expected.\n"),..
        "arma2p",1,"armax"));
    end;
    A=inv_coeff(ar(2));
    if ar(3)<>[] then
        [mb,nb]=size(ar(3));
        B=inv_coeff(ar(3),(nb/ar("nu"))-1);
    else
        B=[];
    end
    D=inv_coeff(ar(4));
endfunction
