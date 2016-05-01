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

function d = assert_computedigits ( varargin )
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

    function d = computedigits_data ( expected , computed , basis )
        //
        dmin = 0
        dmax = -log(2^(-53))/log(basis)
        d = ones(expected)*%inf;
        d( isnan(expected) & isnan(computed) ) = dmax;
        d( isnan(expected) & ~isnan(computed) ) = dmin;
        d( ~isnan(expected) & isnan(computed) ) = dmin;
        // From now, both expected and computed are non-nan
        k = ~isnan(expected) & ~isnan(computed);
        d( k & expected == 0 & computed == 0 ) = dmax;
        d( k & expected == 0 & computed <> 0 ) = dmin;
        // From now, expected is non-zero
        k = k & expected <> 0;
        d( k & expected == computed ) = dmax;
        // From now, expected and computed are different
        k = k & expected <> computed;
        d( k & expected == %inf & computed <> %inf ) = dmin;
        d( k & expected == -%inf & computed <> -%inf ) = dmin;
        // From now, neither of computed, nor expected is infinity
        kdinf=find(d==%inf);
        if ( kdinf <> [] ) then
            relerr = ones(expected)*%nan;
            relerr(kdinf) = abs(computed(kdinf)-expected(kdinf)) ./ abs(expected(kdinf));
            k = find( relerr >= 1 );
            if ( k<> [] ) then
                d(k) = dmin;
            end
            k = find( d==%inf & relerr < 1 );
            if ( k<> [] ) then
                sigdig(k) = -log ( 2*relerr(k) ) ./ log(basis);
                d(k) = max ( sigdig(k) , dmin );
            end
        end
    endfunction
    // Returns the number of significant digits in computed result.

    [lhs,rhs]=argn()
    if ( and ( rhs <> [2 3] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.\n") , "assert_computedigits" , 2 , 3 )
        error(errmsg)
    end
    computed = varargin ( 1 )
    expected = varargin ( 2 )
    basis = assert_argindefault ( rhs , varargin , 3 , 10 )
    //
    // Check types of variables
    if ( typeof(computed) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_computedigits" , 1 )
        error(errmsg)
    end
    if ( typeof(expected) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_computedigits" , 2 )
        error(errmsg)
    end
    //
    // Check sizes of variables
    if ( size(expected) <> size(computed) ) then
        errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n") , "assert_computedigits" , 1 , 2 )
        error(errmsg)
    end
    if ( size(basis,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_computedigits" , 3 , 1 , 1 )
        error(errmsg)
    end
    //
    nre = size(expected,"r")
    nce = size(expected,"c")
    // Update shape
    expected = expected (:)
    computed = computed (:)
    //
    d = zeros(expected)
    //
    dre = computedigits_data ( real(expected) , real(computed) , basis );
    dim = computedigits_data ( imag(expected) , imag(computed) , basis );
    d = min(dre,dim)
    //
    // Reshape
    d = matrix(d,nre,nce)
endfunction
