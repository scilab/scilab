// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function M1=%hm_4_hm(M1,M2)
    if and(M1.dims==M2.dims)==%f then
        error(msprintf(_("%s: Input argument #%d and input argument #%d have incompatible dimensions.\n"),"%hm_4_hm",1,2));
    end
    M1.entries=(M1.entries<=M2.entries)
endfunction
