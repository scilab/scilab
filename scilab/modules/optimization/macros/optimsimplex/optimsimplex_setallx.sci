// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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
