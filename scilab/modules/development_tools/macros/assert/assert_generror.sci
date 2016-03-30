// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function assert_generror ( varargin )
    //   Generates an error.

    [lhs,rhs]=argn()
    if ( and(rhs <> [1 2] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_generror" , 1 , 2 )
        error(errmsg)
    end
    errormsg = varargin(1)
    //
    // Check types of variables
    if ( typeof(errormsg) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_generror" , 1 )
        error(errmsg)
    end
    if ( size(errormsg,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_generror" , 1 , 1 , 1 )
        error(errmsg)
    end
    //
    if ( rhs == 1 ) then
        error ( errormsg )
    else
        errornb = varargin(2)
        if ( size(errornb,"*") <> 1 ) then
            errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_generror" , 2 , 1 , 1 )
            error(errmsg)
        end
        if ( typeof(errornb) <> "constant" ) then
            errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_generror" , 2 )
            error(errmsg)
        end
        if ( errornb < 0 ) then
            errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Non-negative integers expected.\n" ) , "assert_generror" , 2 )
            error(errmsg)
        end
        error ( errormsg , errornb )
    end
endfunction

