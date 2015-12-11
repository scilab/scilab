// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function i=mtlb_int32(x)
    // Emulation function for Matlab int32()

    if x==%inf then
        i=2147483647
    elseif x==-%inf then
        i=-2147483648
    elseif isnan(x) then
        i=0
    else
        i=int32(x)
    end
endfunction
