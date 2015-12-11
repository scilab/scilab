// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
