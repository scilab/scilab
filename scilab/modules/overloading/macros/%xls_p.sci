// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function %xls_p(sheets)
    //displays excel set of sheets
    S=sheets.sheets
    for k=1:size(S)
        V=S(k).value
        name=S(k).name
        mprintf("  %s: %dx%d\n",name,size(V,1),size(V,2))
    end
endfunction
