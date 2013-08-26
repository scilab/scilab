// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function nmplot_summary(nm)
    function str = _strvec ( x )
        str = strcat(string(x)," ")
    endfunction

    xopt = nmplot_get(nm,"-xopt")
    fopt = nmplot_get(nm,"-fopt")
    funevals = nmplot_get(nm,"-funevals")
    iter = nmplot_get(nm,"-iterations")
    status = nmplot_get(nm,"-status")
    mprintf("Xopt: [%s]\n",_strvec(xopt));
    mprintf("Fopt: %s\n",string(fopt));
    mprintf("Function evaluations: %d\n",funevals);
    mprintf("Iterations: %d\n",iter);
    mprintf("Status: %s\n",status);
endfunction
