// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x=tanhm(a)
    //Matrix wise Hyperbolic tangent

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"tanhm",1));
    end
    
    if and(type(a) <> [1, 5]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"), "tanhm", 1));
    end

    if a==[] then x=[],return,end

    [m,n]=size(a);
    if m<>n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"tanhm",1));
    end

    //diagonalization
    [x,t,bs]=bdiag(a+0*%i*ones(a),1/%eps)
    if find(bs>1)<>[] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Matrix is not diagonalisable.\n"),"tanhm",1));
    end

    x=t*diag(tanh(diag(x)))/t;

    if and(imag(a)==0) then x=real(x),end

endfunction
