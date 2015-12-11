// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M2=%hm_q_hm(M1,M2)

    if and(M1("dims")==M2("dims")) then
        M2("entries")=M1("entries").\M2("entries")
    else
        error(msprintf(_("%s: Inconsistent element-wise operation.\n"),"%hm_q_hm"));
    end
endfunction
