// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
