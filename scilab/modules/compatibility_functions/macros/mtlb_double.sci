// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x=mtlb_double(y)

    if type(y)==10 then
        x=asciimat(y)
    elseif type(y)==4 then
        x=bool2s(y)
    elseif type(y)==8 then
        x=double(y)
    else
        x=y
    end
endfunction


