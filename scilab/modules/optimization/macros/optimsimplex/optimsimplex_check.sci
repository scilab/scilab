// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
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
// optimsimplex_check --
//   Check the consistency of the internal data.
//   Generates an error if necessary.
// Arguments
//   <no arg>
//
function optimsimplex_check ( this )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_check", 1));
    end
    nx1 = size(this.x,1)
    nx2 = size(this.x,2)
    if this.nbve<> 0 & nx1 <> this.nbve then
        error(msprintf(gettext("%s: Number of rows of x is %d, which is different from number of vertices = %d."),"optimsimplex_check" , nx1 , this.nbve ));
    end
    if this.n<> 0 & nx2 <> this.n then
        error(msprintf(gettext("%s: Number of columns of x is %d, which is different from dimension = %d."),"optimsimplex_check" , nx2 , this.n ));
    end
    nf1 = size(this.fv,1)
    nf2 = size(this.fv,2)
    if this.n<> 0 & nf1 <> this.nbve then
        error(msprintf(gettext("%s: Number of rows of fv is %d, which is different from number of vertices = %d."),"optimsimplex_check" , nf1 , this.nbve ));
    end
    if this.nbve<> 0 & nf2 <> 1 then
        error(msprintf(gettext("%s: Number of columns of fv is %d, which is different from 1."),"optimsimplex_check" , nf2 ));
    end
endfunction
