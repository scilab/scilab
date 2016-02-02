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

function x=%spb_and(a,flag)
    // or(a) returns the logical OR
    // for scalar matrices, an entry is TRUE if it is not zero.
    //!

    if argn(2)==1 then flag="*",end
    if flag=="*" then
        x=find(~a,1)==[]
    else
        x=sum(bool2s(a),flag)==size(a,flag)
    end
endfunction
