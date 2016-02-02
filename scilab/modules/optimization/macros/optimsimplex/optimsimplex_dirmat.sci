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
// optimsimplex_dirmat --
//   Returns the n x n matrix of simplex directions i.e.
//   the matrix of differences of vertices coordinates
//   with respect to the vertex #1.
// Arguments
//   <no arg>
//
function m = optimsimplex_dirmat ( this )
    nv = this.nbve;
    v1 = this.x(1,1:this.n);
    m(1:this.n,1:nv-1) = (this.x(2:nv,1:this.n) - v1 .*. ones(nv-1,1)).';
endfunction

