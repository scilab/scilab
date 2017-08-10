// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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

function order = assert_comparecomplex ( varargin )
    function order = assert_compdata ( a , b , reltol , abstol )
        if ( a == %inf ) then
            if ( isnan(b) ) then
                order = -1
            elseif ( b == %inf ) then
                order = 0
            else
                order = 1
            end
        elseif ( a == -%inf ) then
            if ( b == -%inf ) then
                order = 0
            else
                order = -1
            end
        elseif ( isnan(a) ) then
            if ( isnan(b) ) then
                order = 0
            else
                order = 1
            end
        else
            if ( isnan(b) ) then
                order = -1
            elseif ( b == -%inf ) then
                order = 1
            elseif ( b == %inf ) then
                order = -1
            else
                areequal = abs ( a - b ) <= reltol * max ( abs(a) , abs(b) ) + abstol
                if ( areequal ) then
                    order = 0
                elseif ( a < b ) then
                    order = -1
                else
                    order = 1
                end
            end
        end
    endfunction


    function argin = assert_argindefault ( rhs , vararglist , ivar , default )
        // Returns the value of the input argument #ivar.
        // If this argument was not provided, or was equal to the
        // empty matrix, returns the default value.
        if ( rhs < ivar ) then
            argin = default
        else
            if ( vararglist(ivar) <> [] ) then
                argin = vararglist(ivar)
            else
                argin = default
            end
        end
    endfunction

    // Compare complex numbers with a tolerance.

    [lhs,rhs]=argn()
    if ( and(rhs <> [2 3 4] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_comparecomplex" , 2 , 4 )
        error(errmsg)
    end
    //
    // Get arguments
    a = varargin(1)
    b = varargin(2)
    reltol = assert_argindefault ( rhs , varargin , 3 , sqrt(%eps) )
    abstol = assert_argindefault ( rhs , varargin , 4 , 0 )
    //
    // Check types of variables
    if ( typeof(a) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_comparecomplex" , 1 )
        error(errmsg)
    end
    if ( typeof(a) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_comparecomplex" , 2 )
        error(errmsg)
    end
    if ( typeof(reltol) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_comparecomplex" , 3 )
        error(errmsg)
    end
    if ( typeof(abstol) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_comparecomplex" , 4 )
        error(errmsg)
    end
    //
    // Check sizes of variables
    if ( size(a,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_comparecomplex" , 1 , 1 , 1 )
        error(errmsg)
    end
    if ( size(b,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_comparecomplex" , 2 , 1 , 1 )
        error(errmsg)
    end
    if ( size(reltol,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_comparecomplex" , 3 , 1 , 1 )
        error(errmsg)
    end
    if ( size(abstol,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_comparecomplex" , 4 , 1 , 1 )
        error(errmsg)
    end
    //
    // Check values of variables
    if ( reltol < 0 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Must be > %d.\n") , "assert_comparecomplex" , 3 , 0 )
        error(errmsg)
    end
    if ( abstol < 0 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Must be > %d.\n") , "assert_comparecomplex" , 4 , 0 )
        error(errmsg)
    end
    //
    ord_re = assert_compdata ( real(a) , real(b) , reltol , abstol )
    if ( ord_re == 0 ) then
        // Tie on the real part: compare imaginary parts
        ord_im = assert_compdata ( imag(a) , imag(b) , reltol , abstol )
        if ( ord_im == 0 ) then
            // Tie on imaginary parts too: two numbers are "equal"
            order = 0
        elseif ( ord_im == -1 ) then
            order = -1
        else
            order = 1
        end
    elseif ( ord_re == -1 ) then
        order = -1
    else
        order = 1
    end
endfunction
