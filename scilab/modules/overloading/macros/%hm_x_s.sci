// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M1=%hm_x_s(M1,M2)
    //author Serge Steer
    if size(M2,"*")<>1 then
        error(msprintf(_("%s: Inconsistent element-wise operation.\n"),"%hm_x_s"));
    else
        M1.entries=M1.entries.*M2
    end
endfunction
