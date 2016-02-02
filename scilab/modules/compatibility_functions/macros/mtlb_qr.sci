// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Q,R,E]=mtlb_qr(A,B)
    // Emulation function for qr() Matlab function

    [lhs,rhs]=argn()

    if lhs<>3 then
        error(msprintf(gettext("%s: Wrong number of output argument(s): %d expected.\n"),"mtlb_qr",3));
    end
    if rhs<>2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"mtlb_qr",2));
    end

    [Q,R,E] = qr(A)
    Q = Q(:,1:min(size(A,2),size(Q,2)))
    if find(size(A)==1)<>[] then
        R = R(1,1:size(A,2))
    end
    E = (1:size(R,2))*E

endfunction
