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
// optimsimplex_setall --
//   Set all the coordinates and the function values of all the vertices.
//   The given matrix is expected to have the following organization
//   * size nbve X n+1
//   * data is organized by row : function value, then x
//   * simplex(k,1) is the function value of the vertex #k, with k = 1 , nbve
//   * simplex(k,2:n+1) is the coordinates of the vertex #k, with k = 1 , nbve
// Arguments
//   simplex : the simplex to set
//
function this = optimsimplex_setall ( this , simplex )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setall", 1));
    end
    if type(simplex) <> 1 | ~isreal(simplex) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setall", 2));
    end
    nbve = size(simplex,1)
    np1 = size(simplex,2)
    if np1 > nbve then
        errmsg = msprintf(gettext ( "%s: The number of vertices (i.e. the number of rows) is %d which is smaller than the number of columns %d (i.e. n+1).") , "optimsimplex_setall",nbve,np1);
        error(errmsg);
    end
    this.n = np1 - 1;
    this.nbve = nbve;
    this.fv ( 1:nbve , 1 ) = simplex ( 1:nbve , 1 )
    this.x ( 1:nbve , 1:this.n ) = simplex ( 1:nbve , 2:this.n+1 )
endfunction
