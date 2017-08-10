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
// optimsimplex_xbar --
//   Returns the center of n vertices, by excluding the
//   vertex with index iexcl.
//   The default value of iexcl is the number of vertices : in that case,
//   if the simplex is sorted in increasing function value
//   order, the worst vertex is excluded.
// Arguments
//   iexcl : the index of the vertex to exclude in
//     center computation.
//
function cen = optimsimplex_xbar ( this , iexcl )
    if (~isdef("iexcl","local")) then
        iexcl = this.nbve;
    end
    if ( size(iexcl,1)<>1 ) then
        errmsg = msprintf(gettext("%s: The exclusion index vector has %d rows instead of 1."), ...
        "optimsimplex_xbar", size(iexcl,1) );
        error(errmsg);
    end
    // Vectorize by making the sum of all, subtracting only one vector
    cen = sum(this.x(1:this.nbve,1:this.n),"r")
    cen = cen - sum(this.x(iexcl,1:this.n),"r")
    nexcl = size(iexcl,2)
    cen = cen / ( this.nbve - nexcl )
endfunction
