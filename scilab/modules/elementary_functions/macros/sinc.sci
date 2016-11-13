// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=sinc(x,fl)
    //               [  sin(x(i))/x(i) if x(i)~=0
    // computes y(i)=[
    //               [  1 if x(i)~=0

    rhs = argn(2);
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"sinc", 1));
    end
    
    if and(typeof(x)<>["constant", "sparse", "hypermat"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"),"sinc",1));
    end

    if argn(2) == 2 then // for compatibility
        warning("obsolete use of sinc, use filt_sinc instead")
        ffilt = ffilt; //load ffilt and its subfunctions
        y = filt_sinc(x, fl);
        return
    end
    y = ones(x);
    kz = find(x <> 0);
    y(kz) = sin(x(kz))./(x(kz));
endfunction
