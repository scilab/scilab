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
// optimsimplex_getall --
//   Returns all the coordinates of all the vertices and the function values
//   in the same matrix, with size nbve X n+1, which is organized as follows :
//   * data is organized by row : function value, then x
//   * simplex(k,1) is the function value of the vertex #k, with kve = 1 , nbve
//   * simplex(k,2:n+1) is the coordinates of the vertex #k, with kve = 1 , nbve
// Arguments
//   <no arg>
//
function simplex = optimsimplex_getall ( this )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getall", 1));
    end
    simplex = zeros ( this.nbve , this.n+1 );
    simplex ( 1:this.nbve , 1 ) = this.fv ( 1:this.nbve , 1 )
    simplex ( 1:this.nbve , 2:this.n+1 ) = this.x ( 1:this.nbve , 1:this.n )
endfunction
