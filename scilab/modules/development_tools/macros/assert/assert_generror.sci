// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

