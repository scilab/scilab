// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=mtlb_eye(a)
    // Emulation function for eye() Matlab function

    if and(size(a)==[1 1]) then
        r=eye(a,a)
    else
        r=eye(a(1),a(2))
    end
endfunction
