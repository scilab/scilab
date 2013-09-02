//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function %diagram_xcos(scs_m)
    // Overload function when calling xcos with a diagram
    //
    // Calling Sequence
    //  xcos(scs_m);
    //
    // Parameters
    //  scs_m: the diagram instance

    // allocate a local copy
    scs_m = scs_m;
    // call xcos with the copy and name
    xcos(scs_m, "scs_m");
endfunction
