// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
