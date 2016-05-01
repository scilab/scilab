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

function rtol = assert_cond2reltol ( varargin )
    // Suggests a relative error, computed from the condition number.

    function varargout = assert_expandvar ( varargin )
        // Expand variables so that they all have the same shape.
        //
        // Syntax
        //   ovar1 = assert_expandvar ( ivar1 )
        //   [ ovar1 , ovar2 ] = assert_expandvar ( ivar1 , ivar2 )
        //   [ ovar1 , ovar2 , ovar3 ] = assert_expandvar ( ivar1 , ivar2 , ivar3 )
        //   [ ovar1 , ovar2 , ovar3 , ovar4 ] = assert_expandvar ( ivar1 , ivar2 , ivar3 , ivar4 )
        //
        // Parameters
        //   ivar1 : input variable #1
        //   ovar1 : output variable #1
        //
        // Description
        //   This function allows to expand input arguments of
        //   computationnal functions.
        //   If scalars are input, expand it to the size of other other variables.
        //
        //   If matrices are input, all must have the same shape: if not, an error is generated.
        //
        // Examples
        //   // Expand ovar1 to [1 1 1]
        //   [ ovar1 , ovar2 ] = assert_expandvar ( 1 , [2 3 4] )
        //
        //   // Expand ovar2 to [4 4 4]
        //   [ ovar1 , ovar2 ] = assert_expandvar ( [1 2 3] , 4 )
        //
        //   // Expand ovar2 to [4 4 4]'
        //   [ ovar1 , ovar2 ] = assert_expandvar ( [1 2 3]' , 4 )
        //
        // Authors
        //   Michael Baudin - 2009-2010 - DIGITEO

        [lhs,rhs]=argn()
        if ( rhs <> lhs ) then
            errmsg = sprintf ( gettext ( "%s: The number of output arguments %d do not match the number of input arguments %d.") , "assert_expandvar" , lhs , rhs )
            error(errmsg)
        end

        //
        // Check if there is one argument which is a matrix.
        // imat is the index of the input argument which is a matrix.
        istherematrix = %f
        imat = 0
        for ivar = 1 : rhs
            if ( size ( varargin(ivar) , "*" ) <> 1 ) then
                istherematrix = %t
                imat = ivar
                break
            end
        end
        // If there is no matrix, returns the output arguments as is.
        if ( ~istherematrix ) then
            for ovar = 1 : lhs
                varargout ( ovar ) = varargin ( ovar )
            end
            return
        end
        // If there is one matrix, get its size.
        nbrows = size ( varargin ( imat ) , "r" )
        nbcols = size ( varargin ( imat ) , "c" )
        // Check that all matrices have the same shape.
        for ivar = 1 : rhs
            nbi = size ( varargin ( ivar ) , "*" )
            if ( nbi <> 1 ) then
                nbrowsi = size ( varargin ( ivar ) , "r" )
                nbcolsi = size ( varargin ( ivar ) , "c" )
                if ( nbrowsi <> nbrows | nbcolsi <> nbcols ) then
                    errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "assert_expandvar" , ivar , nbrows , nbcols );
                    error(errmsg)
                end
            end
        end
        // Expand all input arguments which are scalar up to this shape.
        for ivar = 1 : rhs
            if ( size ( varargin(ivar) , "*" ) == 1 ) then
                varargin ( ivar ) = varargin ( ivar ) * ones ( nbrows , nbcols )
            end
        end
        // Set all output variables
        for ovar = 1 : lhs
            varargout ( ovar ) = varargin ( ovar )
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


    [lhs,rhs]=argn()
    if ( and ( rhs <> [1 2] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_cond2reltol" , 1 , 2 )
        error(errmsg)
    end
    //
    // Get arguments
    condition = varargin ( 1 )
    offset = assert_argindefault ( rhs , varargin , 2 , 0 )
    //
    // Check types of variables
    if ( typeof(condition) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_cond2reltol" , 1 )
        error(errmsg)
    end
    if ( typeof(offset) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_cond2reltol" , 2 )
        error(errmsg)
    end
    //
    // Check size of variables
    // Let the user and assert_expandvar manage this.
    //
    // Check content of variables
    if ( or ( condition < 0 ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Must be > %d.\n") , "assert_cond2reltol" , 1 , 0 )
        error(errmsg)
    end
    //
    [condition,offset] = assert_expandvar (condition,offset)
    //
    d = assert_cond2reqdigits ( condition , offset , 10 )
    rtol = 10^-d
endfunction

