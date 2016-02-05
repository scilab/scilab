// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
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
// optimsimplex_new --
//   Creates a new simplex object.
// Note
//   * n is the dimension of the space
//   * one vertex is a point in the space, defined
//     by its n coordinates and associated with a function
//     value
//   * the simplex is made of k>=n+1 vertices
//   * the function value for vertex #kve is in fv(kve),
//     with kve = 1 , k
//   * the vertex #kve is stored in x(kve,1:n)
//     with kve = 1 , k
// Arguments
//   coords : list of point coordinates in the simplex
//   fun : the function to compute at vertices
//   data : user-defined data, passed to the function
// Uses :
//   newobj = optimsimplex_new ( )
//   newobj = optimsimplex_new ( coords )
//   newobj = optimsimplex_new ( coords , fun )
//   [ newobj , data ] = optimsimplex_new ( coords , fun , data )
//   newobj = optimsimplex_new ( "axes" , x0 )
//   newobj = optimsimplex_new ( "axes" , x0 , fun )
//   newobj = optimsimplex_new ( "axes" , x0 , fun , len )
//   [ newobj , data ] = optimsimplex_new ( "axes" , x0 , fun , len , data )
//   newobj = optimsimplex_new ( "spendley" , x0 )
//   newobj = optimsimplex_new ( "spendley" , x0 , fun )
//   newobj = optimsimplex_new ( "spendley" , x0 , fun , len )
//   [ newobj , data ] = optimsimplex_new ( "spendley" , x0 , fun , len , data )
//   newobj = optimsimplex_new ( "pfeffer" , x0 )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero )
//   [ newobj , data ] = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero , data )
//   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve )
//   [ newobj , data ] = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve  , data )
//   newobj = optimsimplex_new ( "oriented" , simplex0 )
//   newobj = optimsimplex_new ( "oriented" , simplex0 , fun )
//   [ newobj , data ] = optimsimplex_new ( "oriented" , simplex0 , fun , data )
//
function [ newobj , data ] = optimsimplex_new ( varargin )
    function newobj = optimsimplex_coords ( varargin )
        // newobj = optimsimplex_coords (  )
        // newobj = optimsimplex_coords ( coords )
        [lhs,rhs]=argn();
        if ( rhs> 1 ) then
            errmsg = msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "optimsimplex_coords", 0,1);
            error(errmsg)
        end
        coords = optim_argindefault ( rhs , varargin , 1 , [] )
        assert_typereal ( coords , "coords" , 1 );
        newobj = tlist(["TSIMPLEX",...
        "verbose","x","n","fv","nbve"]);
        newobj.verbose = 0;
        // The dimension of the space
        newobj.n = 0;
        // The number of vertices
        newobj.nbve = 0;
        // The coordinates of the vertices, with size nbve x n
        newobj.x = [];
        // The function values, with size nbve x 1
        newobj.fv = [];
        //
        // Take input arguments into account
        //
        if ( coords<>[] ) then
            nbve = size(coords,1)
            n = size(coords,2)
            if nbve < n + 1 then
                error(msprintf(gettext("%s: The numbers of columns of coords is %d but is expected to be at least %d"),...
                "optimsimplex_new" , nbve , n + 1))
            end
            newobj.n = n;
            newobj.nbve = nbve;
            newobj.x(1:nbve,1:n) = coords(1:nbve,1:n);
        end
    endfunction
    //
    // optimsimplex_axes --
    //   Configure the current simplex so that it is computed from the axes and a length.
    // Arguments
    //   x0 : the initial point, as a row vector
    //   length : the length of the simplex
    //     If length is a value, that unique length
    //     is used in all directions.
    //     If length is a vector with n values, each
    //     length is used with the corresponding
    //     direction.
    //
    function newobj = optimsimplex_axes ( x0 , len )
        newobj = optimsimplex_coords ( )
        n = length(x0);
        newobj.n = n;
        newobj.nbve = n + 1;
        nl=length(len)
        if ( nl==1 ) then
            xlen = len * ones(1,n)
        else
            xlen = len
        end
        newobj.x = zeros ( newobj.nbve , n )
        newobj.fv = zeros ( newobj.nbve , 1 )
        //
        // Set all points
        //
        nv = newobj.nbve;
        newobj.x ( 1:nv , : ) = x0 (1:n) .*. ones(nv,1);
        newobj.x ( 2:nv , : ) = newobj.x ( 2:nv , : ) + diag(xlen);
    endfunction

    //
    // optimsimplex_spendley --
    //   Configure the current simplex so that it is computed from Spendley's method,
    //   i.e. a regular simplex made of k = n+1 vertices.
    // Arguments
    //   x0 : the initial point, as a row vector
    //   len : the length of the simplex
    //
    function newobj = optimsimplex_spendley ( x0 , len )
        newobj = optimsimplex_coords ( )
        n = length(x0);
        newobj.n = n;
        newobj.nbve = n + 1;
        newobj.x = zeros ( n+1  , n )
        newobj.fv = zeros ( n+1 , 1 )
        //
        // Compute p (diagonal term) , q (off-diagonal term)
        //
        p  = (n - 1.0 + sqrt(n + 1))/(n * sqrt(2.0))
        q = (sqrt(n + 1) - 1.0)/(n * sqrt(2.0))
        //
        // Set all points
        //
        nv = newobj.nbve;
        newobj.x ( 1:nv , : ) = x0 (1:n) .*. ones(nv,1);
        newobj.x ( 2:nv , : ) = newobj.x ( 2:nv , : ) + diag(ones(n,1)*p) + q*ones(n,n) - diag(ones(n,1)*q);
    endfunction

    //
    // optimsimplex_pfeffer --
    //   Configure the current simplex so that it is computed from Pfeffer's method,
    //   i.e. a relative delta for non-zero values and an absolute delta
    //   for zero values.
    // Arguments
    //   x0 : the initial point, as a row vector
    //   deltausual : the absolute delta for non-zero values
    //   deltazero : the absolute delta for zero values
    // References
    //   "Global Optimization Of Lennard-Jones Atomic Clusters"
    //   Ellen Fan, Thesis, February 26, 2002, McMaster University
    //   Method due to L. Pfeffer at Stanford
    //
    function newobj = optimsimplex_pfeffer ( x0 , deltausual , deltazero )
        newobj = optimsimplex_coords ( )
        n = length(x0);
        newobj.n = n;
        newobj.nbve = n + 1;
        newobj.x = zeros ( n+1 , n )
        newobj.fv = zeros ( n+1 , 1 )
        //
        // Set 1st point
        //
        newobj.x ( 1 , 1:n ) = x0 (1,1:n)
        //
        // Set points #2 to #n+1
        //
        for j = 2 : newobj.n+1
            newobj.x ( j,1:n ) = x0 (1:n)
            if ( x0( j-1 ) == 0.0 ) then
                newobj.x ( j , j-1 ) = deltazero
            else
                newobj.x ( j , j-1 ) = newobj.x ( j , j-1 ) + deltausual * x0( j-1 )
            end
        end
    endfunction

    //
    // optimsimplex_randbounds --
    //   Configure the current simplex so that it is computed by taking the bounds
    //   into account with random scaling.
    // Arguments
    //   x0 : the initial point
    //   boundsmin : array of minimum bounds
    //   boundsmax : array of maximum bounds
    //   nbve : total number of vertices in the simplex
    //
    function newobj = optimsimplex_randbounds ( x0 , boundsmin , boundsmax , nbve )
        newobj = optimsimplex_coords ( )
        newobj.n = n;
        newobj.nbve = nbve;
        newobj.x = zeros ( nbve , n )
        newobj.fv = zeros ( nbve , 1 )
        //
        // Set 1st point
        //
        newobj.x ( 1 , 1:n ) = x0 (1:n)
        //
        // Set points #2 to #nbve, by randomizing the bounds
        //
        bminmat = boundsmin( 1,1:n ) .*. ones(nbve-1,1);
        bmaxmat = boundsmax( 1,1:n ) .*. ones(nbve-1,1);
        thetas = rand(n,nbve-1);
        newobj.x ( 2:nbve , 1:n ) = bminmat + (thetas.') .* (bmaxmat - bminmat)
    endfunction

    //
    // optimsimplex_oriented --
    //   Returns an oriented simplex, in sorted order.
    //   This simplex may be used, as Kelley suggests
    //   for a restart of Nelder-Mead algorithm.
    // Arguments
    //   fun : the function to compute at vertices
    //
    function newobj = optimsimplex_oriented ( simplex0 )
        sgrad = optimsimplex_gradientfv ( simplex0 )
        ssize = optimsimplex_size ( simplex0 , "sigmaminus" )
        n = simplex0.n
        // Compute the betas
        ipos = find(sgrad >= 0.0);
        ineg = find(sgrad < 0.0);
        betav(ipos) = ssize;
        betav(ineg) = -ssize;
        betav = -0.5 * betav
        // Prepare a matrix with beta as diagonal terms
        mid = diag(betav);
        // Compute simplex
        newobj = optimsimplex_new()
        newobj.n = simplex0.n
        newobj.nbve = simplex0.n+1
        newobj.x = zeros ( n+1 , n )
        newobj.fv = zeros ( n+1 , 1 )
        // Store all points
        x1 = simplex0.x ( 1 , 1:n )
        newobj.x ( 1:n+1, 1:n ) = x1 ( 1 , 1:n ) .*. ones(n+1,1)
        // Retrieve the function value for the first simplex
        // This saves one function evaluation
        newobj.fv ( 1 ) = simplex0.fv ( 1 )
        newobj.x ( 2:n+1, 1:n ) = mid ( 1:n , 1:n ) + newobj.x ( 2:n+1, 1:n )
    endfunction

    // Generates an error if the given variable is not of type real
    function assert_typereal ( var , varname , ivar )
        if ( type ( var ) <> 1 ) then
            errmsg = msprintf(gettext("%s: Expected real variable for variable %s at input #%d, but got %s instead."),"assert_typereal", varname , ivar , typeof(var) );
            error(errmsg);
        end
    endfunction
    // Generates an error if the given variable is not of type string
    function assert_typestring ( var , varname , ivar )
        if ( type ( var ) <> 10 ) then
            errmsg = msprintf(gettext("%s: Expected string variable for variable %s at input #%d, but got %s instead."),"assert_typestring", varname , ivar , typeof(var) );
            error(errmsg);
        end
    endfunction
    // Generates an error if the given variable is not of type function (macro)
    function assert_typefunction ( var , varname , ivar )
        if ( and(type ( var ) <> [11 13] )) then
            errmsg = msprintf(gettext("%s: Expected function but for variable %s at input #%d, got %s instead."),"assert_typefunction", varname , ivar , typeof(var) );
            error(errmsg);
        end
    endfunction
    // Generates an error if the given variable is not of type boolean
    function assert_typeboolean ( var , varname , ivar )
        if ( type ( var ) <> 4 ) then
            errmsg = msprintf(gettext("%s: Expected boolean but for variable %s at input #%d, got %s instead."),"assert_typeboolean", varname , ivar , typeof(var) );
            error(errmsg);
        end
    endfunction

    // Generates an error if the value corresponding to an option is unknown.
    function unknownValueForOption ( value , optionname )
        errmsg = msprintf(gettext("%s: Unknown value %s for %s option"),"unknownValueForOption",value , optionname );
        error(errmsg);
    endfunction

    function argin = optim_argindefault ( rhs , vararglist , ivar , default )
        // Returns the value of the input argument #ivar.
        // If this argument was not provided, or was equal to the
        // empty matrix, returns the default value.
        if ( rhs < ivar ) then
            argin = default
        else
            if ( typeof(vararglist(ivar))== "constant" ) then
                if ( vararglist(ivar) <> [] ) then
                    argin = vararglist(ivar)
                else
                    argin = default
                end
            else
                argin = vararglist(ivar)
            end
        end
    endfunction

    [lhs,rhs]=argn();
    if rhs>7 then
        errmsg = msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "optimsimplex_new", 0,7);
        error(errmsg)
    end
    if rhs == 0 then
        newobj = optimsimplex_coords ( )
        return;
    end
    var1 = varargin(1);
    data = []
    if typeof(var1) == "string" then
        stype = varargin(1);
        select stype
        case "axes" then
            //   newobj = optimsimplex_new ( "axes" , x0 )
            //   newobj = optimsimplex_new ( "axes" , x0 , fun )
            //   newobj = optimsimplex_new ( "axes" , x0 , fun , len )
            //   [ newobj , data ] = optimsimplex_new ( "axes" , x0 , fun , len , data )
            if rhs<2 | rhs > 5 then
                errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 5 are expected."), "optimsimplex_new", rhs);
                error(errmsg)
            end
            x0   = varargin(2);
            fun = optim_argindefault ( rhs , varargin , 3 , [] )
            len = optim_argindefault ( rhs , varargin , 4 , 1.0 )
            data = optim_argindefault ( rhs , varargin , 5 , [] )
            //
            // Check inputs
            //
            assert_typereal ( x0 , "x0" , 3 );
            assert_typereal ( len , "len" , 1 );
            if size(x0,1)<>1 then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",2,1,size(x0,2));
                error(errmsg);
            end
            if ( fun <> [] ) then
                assert_typefunction ( fun , "fun" , 2 );
            end
            if ( size(len,1)<>1 ) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",4,1,size(len,2));
                error(errmsg);
            end
            if ( size(len,"*") <> 1 )
                if ( size ( len , 2 ) <> size ( x0 , 2 ) ) then
                    errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",4,size(len,1),size(x0,2));
                    error(errmsg);
                end
            end
            newobj = optimsimplex_axes ( x0 , len )
        case "spendley" then
            //   newobj = optimsimplex_new ( "spendley" , x0 )
            //   newobj = optimsimplex_new ( "spendley" , x0 , fun )
            //   newobj = optimsimplex_new ( "spendley" , x0 , fun , len )
            //   [ newobj , data ] = optimsimplex_new ( "spendley" , x0 , fun , len , data )
            if rhs<2 | rhs > 5 then
                errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 5 are expected."), "optimsimplex_new", rhs);
                error(errmsg)
            end
            x0   = varargin(2);
            fun = optim_argindefault ( rhs , varargin , 3 , [] )
            len = optim_argindefault ( rhs , varargin , 4 , 1.0 )
            data = optim_argindefault ( rhs , varargin , 5 , [] )
            //
            // Check inputs
            //
            assert_typereal ( x0 , "x0", 1 );
            if ( fun <> [] ) then
                assert_typefunction ( fun , "fun" , 2 );
            end
            assert_typereal ( len , "len" , 3 );
            if size(x0,1)<>1 then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",2,1,size(x0,2));
                error(errmsg);
            end
            if ( size(len)<>[1  1] ) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",4,1,1);
                error(errmsg);
            end
            newobj = optimsimplex_spendley ( x0 , len )
        case "pfeffer" then
            //   newobj = optimsimplex_new ( "pfeffer" , x0 )
            //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun )
            //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual )
            //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero )
            //   [ newobj , data ] = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero , data )
            if rhs<2 | rhs > 6 then
                errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 6 are expected."), "optimsimplex_new", rhs);
                error(errmsg)
            end
            x0   = varargin(2);
            fun = optim_argindefault ( rhs , varargin , 3 , [] )
            deltausual = optim_argindefault ( rhs , varargin , 4 , 0.05 )
            deltazero = optim_argindefault ( rhs , varargin , 5 , 0.0075 )
            data = optim_argindefault ( rhs , varargin , 6 , [] )
            //
            // Check inputs
            //
            assert_typereal ( x0 , "x0" , 1 );
            assert_typereal ( deltausual , "deltausual", 3 );
            assert_typereal ( deltazero , "deltazero" , 4 );
            if size(x0,1)<>1 then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",2,1,size(x0,2));
                error(errmsg);
            end
            if (size(deltausual)<>[1 1]) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",3,1,1);
                error(errmsg);
            end
            if (size(deltazero)<>[1 1]) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",4,1,1);
                error(errmsg);
            end
            if ( fun <> [] ) then
                assert_typefunction ( fun , "fun" , 2 );
            end
            newobj = optimsimplex_pfeffer ( x0 , deltausual , deltazero )
        case "randbounds" then
            //   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax )
            //   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve )
            //   [ newobj , data ] = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve  , data )
            if rhs<5 | rhs > 7 then
                errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 6 to 7 are expected."), "optimsimplex_new", rhs);
                error(errmsg)
            end
            x0   = varargin(2);
            fun  = varargin(3);
            boundsmin = varargin(4);
            boundsmax = varargin(5);
            n = length ( x0 )
            nbve = optim_argindefault ( rhs , varargin , 6 , n+1 )
            data = optim_argindefault ( rhs , varargin , 7 , [] )
            //
            // Check inputs
            //
            assert_typereal ( x0 , "x0", 1 );
            if ( fun <> [] ) then
                assert_typefunction ( fun , "fun" , 2 );
            end
            assert_typereal ( boundsmin , "boundsmin" , 3 );
            assert_typereal ( boundsmax , "boundsmax" , 4 );
            assert_typereal ( nbve , "nbve" , 5 );
            if ( size(x0,1)<>1 ) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",2,1,size(x0,2));
                error(errmsg);
            end
            if ( size(boundsmin,1)<>1 | size(boundsmin,2)<>size(x0,2)) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",4,1,size(x0,2));
                error(errmsg);
            end
            if ( size(boundsmax,1)<>1 | size(boundsmax,2)<>size(x0,2)) then
                errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"optimsimplex_new",5,1,size(x0,2));
                error(errmsg);
            end

            newobj = optimsimplex_randbounds ( x0 , boundsmin , boundsmax , nbve )
        case "oriented" then
            //   newobj = optimsimplex_new ( "oriented" , simplex0 )
            //   newobj = optimsimplex_new ( "oriented" , simplex0 , fun )
            //   [ newobj , data ] = optimsimplex_new ( "oriented" , simplex0 , fun , data )
            if rhs<2 | rhs > 4 then
                errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 4 are expected."), "optimsimplex_new", rhs);
                error(errmsg)
            end
            simplex0   = varargin(2);
            fun = optim_argindefault ( rhs , varargin , 3 , [] )
            data = optim_argindefault ( rhs , varargin , 4 , [] )
            //
            // Check inputs
            //
            if (typeof(simplex0)<> "TSIMPLEX") then
                errmsg = msprintf(gettext ( "%s: Wrong type for argument #%d: %s expected.\n") , "optimsimplex_new",2,"TSIMPLEX")
                error(errmsg)
            end
            if (fun <> [] ) then
                assert_typefunction ( fun , "fun" , 2 );
            end
            if ( simplex0.nbve == [] ) then
                errmsg = msprintf(gettext ( "%s: The initial simplex has no vertices.") , "optimsimplex_new")
                error(errmsg)
            end
            if ( simplex0.n == [] ) then
                errmsg = msprintf(gettext ( "%s: The initial simplex has no dimension.") , "optimsimplex_new")
                error(errmsg)
            end
            if ( simplex0.nbve <> simplex0.n+1 ) then
                errmsg = msprintf(gettext ( "%s: The oriented simplex can be computed only with a simplex made of n+1 points, but the dimension is %d and the number of vertices is %d") , "optimsimplex_new", simplex0.n , simplex0.nbve)
                error(errmsg)
            end

            newobj = optimsimplex_oriented ( simplex0 )
        else
            errmsg = msprintf(gettext("%s: Unknown key %s"),"optimsimplex_new",key)
            error(errmsg)
        end
    else
        //   newobj = optimsimplex_new ( coords )
        //   newobj = optimsimplex_new ( coords , fun )
        //   [ newobj , data ] = optimsimplex_new ( coords , fun , data )
        if rhs < 1 | rhs > 3 then
            errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 1 to 3 are expected."), "optimsimplex_new", rhs);
            error(errmsg)
        end
        coords = varargin(1);
        fun = optim_argindefault ( rhs , varargin , 2 , [] )
        data = optim_argindefault ( rhs , varargin , 3 , [] )
        //
        // Check inputs
        //
        if (fun <> [] ) then
            assert_typefunction ( fun , "fun" , 2 );
        end
        newobj = optimsimplex_coords ( coords )
    end
    //
    // Compute function values, if required.
    if ( fun<>[] ) then
        if ( typeof(data)=="constant" ) then
            if ( data==[] ) then
                newobj = optimsimplex_computefv ( newobj , fun )
            else
                [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
            end
        else
            [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
        end
    end
endfunction

