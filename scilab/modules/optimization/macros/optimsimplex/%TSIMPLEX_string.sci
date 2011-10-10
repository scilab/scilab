// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

