// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// %TSIMPLEX_string --
//   Returns the string containing the Optim Simplex component.
//
function str = %TSIMPLEX_string ( this )
    function str = mysize(x)
        [n,m]=size(x)
        if ( n==0 & m==0 ) then
            str = sprintf("[] matrix");
        else
            str = sprintf("%d-by-%d matrix\n",n,m);
        end
    endfunction

    str = []
    k = 1
    str(k) = sprintf("Optim Simplex Object:\n")
    k = k + 1
    str(k) = sprintf("=====================")
    k = k + 1
    str(k) = sprintf("nbve: %d\n",this.nbve);
    k = k + 1
    str(k) = sprintf("n: %d\n",this.n);
    k = k + 1
    str(k) = sprintf("x: %s\n",mysize(this.x));
    k = k + 1
    str(k) = sprintf("fv: %s\n",mysize(this.fv));
endfunction

