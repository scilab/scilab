// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -  F. Delebecque
// Copyright (C) INRIA -  S. Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function out=generic_i_st(i,in,out)

    // out.i=in
    // this function is called only if the i field is not already defined in out
    //
    if prod(out.dims)>1 then
        error(1);
    end
    T=getfield(1,out);
    T($+1)=i;out=setfield(1,T,out)
    out=setfield($+1,in,out)
    if double(out.dims)==[0 0] then
        out.dims=int32([1 1]);
    end
endfunction
