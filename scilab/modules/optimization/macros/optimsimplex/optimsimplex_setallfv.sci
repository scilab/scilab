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
// optimsimplex_setallfv --
//   Set all the function values of all the vertices.
//   The vertex #k is expected to be stored in fv(k)
//   with k = 1 , nbve
// Arguments
//   fv : the array of function values
//
function this = optimsimplex_setallfv ( this , fv )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setallfv", 1));
    end
    if type(fv) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real vector expected.\n"), "optimsimplex_setallfv", 2));
    end
    fv1 = size ( fv , 1 );
    if fv1 <> this.nbve then
        error ( msprintf ( gettext ( "%s: The number of rows in the function value array is %d, while expected %d." ), "optimsimplex_setallfv" , fv1 , this.nbve ))
    end
    fv2 = size ( fv , 2 );
    if fv2 <> 1 then
        error ( msprintf ( gettext ( "%s: The number of columns in the function value array is %d, while expected 1." ), "optimsimplex_setallfv" , fv2 ))
    end
    this.fv ( 1:this.nbve , 1 ) = fv ( 1:this.nbve );
endfunction
