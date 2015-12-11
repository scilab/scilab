// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// optimsimplex_setallx --
//   Set all the coordinates of all the vertices.
//   The vertex #k is expected to be stored in x(k,1:n)
//   with k = 1 , nbve
// Arguments
//   x : the coordinates of the vertices.
//
function this = optimsimplex_setallx ( this , x )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setallx", 1));
    end
    if type(x) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setallx", 2));
    end
    nx1 = size ( x , 1 );
    if nx1 <> this.nbve then
        error ( msprintf ( gettext ( "%s: The number of rows in x is %d, while expected %d." ), "optimsimplex_setallx" , nx1 , this.nbve ))
    end
    nx2 = size ( x , 2 );
    if nx2 <> this.n then
        error ( msprintf ( gettext ( "%s: The number of columns in x is %d, while expected %d." ), "optimsimplex_setallx" , nx2 , this.n ))
    end
    this.x ( 1:this.nbve , 1:this.n ) = x ( 1:this.nbve , 1:this.n );
endfunction
