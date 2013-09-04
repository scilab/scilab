// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function t=acoshm(x)

    //Matrix wise Hyberbolic cosine inverse of x

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"acoshm",1));
    end

    if type(x)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"acoshm",1));
    end

    [m,n]=size(x);

    if m<>n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"acoshm",1));
    end

    t=logm(x+(x+eye())*sqrtm((x-eye())/(x+eye())));

endfunction
