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

function %xls_p(sheets)
    //displays excel set of sheets
    S=sheets.sheets
    for k=1:size(S)
        V=S(k).value
        name=S(k).name
        mprintf("  %s: %dx%d\n",name,size(V,1),size(V,2))
    end
endfunction
