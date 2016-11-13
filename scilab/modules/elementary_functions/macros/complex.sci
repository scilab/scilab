// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  y=complex(varargin)

    // COMPLEX function
    // Given the real part and the imaginary part this function constructs the complex form : y=complex(a,b) returns y=a+b*i
    // Inputs :
    //  a, b : real scalars/vectors/matrices
    // output :
    // y : complex form

    [lhs,rhs]=argn(0)
    if ( rhs<1 | rhs>2 ) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"complex",1,2));
    end

    // Get input arguments
    a = varargin(1)
    if ( rhs == 1 ) then
        b = zeros(a)
    else
        b = varargin(2)
    end
    //
    // Check type of input arguments
    if ( typeof(a)<>"constant" ) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"),"complex",1));
    end
    if ( typeof(b)<>"constant" ) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"),"complex",2));
    end
    //
    // Check size
    if ( size(a,"*") <> 1 & size(b,"*") <> 1 & size(a)<>size(b) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"complex",1,2));
    end
    //
    // Check content of input arguments
    if ( ~isreal(a) ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"complex",1));
    end
    if ( ~isreal(b) ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"complex",2));
    end

    // Proceed...
    y = a+imult(b);
endfunction
