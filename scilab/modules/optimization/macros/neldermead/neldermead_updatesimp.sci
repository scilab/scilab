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
// neldermead_updatesimp --
//   Update the initial simplex simplex0 for a restart.
// Arguments
//   <>
//
function this = neldermead_updatesimp ( this )
    simplex0 = optimsimplex_new ( );
    xopt = optimbase_get ( this.optbase , "-xopt" );
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    //
    // Check the consistency between the initial simplex method
    // and the restart simplex method.
    simplex0method = neldermead_cget ( this , "-simplex0method" )
    restartsimplexmethod = neldermead_cget ( this , "-restartsimplexmethod" )
    select restartsimplexmethod
    case "oriented" then
        // An oriented restart is possible only if the initial simplex has n+1 vertices.
        if ( simplex0method == "randbounds" ) then
            lclmsg = gettext("%s: The initial simplex method ""%s"" is not compatible with the restart simplex method ""%s""")
            errmsg = msprintf(lclmsg, "neldermead_updatesimp", simplex0method, restartsimplexmethod)
            error(errmsg)
        end
    case "axes" then

    case "spendley" then

    case "pfeffer" then

    case "randbounds" then
        if ( ~hasbounds ) then
            lclmsg = gettext("%s: Randomized bounds restart simplex is not available without bounds." )
            errmsg = msprintf ( lclmsg , "neldermead_updatesimp" )
            error ( errmsg )
        end
    else
        errmsg = msprintf(gettext("%s: Unknown restart simplex method %s"), "neldermead_updatesimp", this.restartsimplexmethod)
        error(errmsg)
    end
    //
    // Update the simplex
    select this.restartsimplexmethod
    case "oriented" then
        [ simplex0 , this ] = optimsimplex_new ( "oriented" , this.simplexopt , costf_transposex , this );
    case "axes" then
        simplex0 = optimsimplex_destroy ( simplex0 )
        [ simplex0 , this ] = optimsimplex_new ( "axes" , ...
        xopt.' , costf_transposex , this.simplex0length , this );
    case "spendley" then
        simplex0 = optimsimplex_destroy ( simplex0 )
        [ simplex0 , this ] = optimsimplex_new ( "spendley" , ...
        xopt.' , costf_transposex , this.simplex0length , this );
    case "pfeffer" then
        simplex0 = optimsimplex_destroy ( simplex0 )
        [ simplex0 , this ] = optimsimplex_new ( "pfeffer" , ...
        xopt.' , costf_transposex , this.simplex0deltausual , ...
        this.simplex0deltazero , this );
    case "randbounds" then
        if ( this.boxnbpoints=="2n" ) then
            nbvar=optimbase_cget(this.optbase,"-numberofvariables")
            this.boxnbpointseff = 2*nbvar
        else
            this.boxnbpointseff = this.boxnbpoints
        end
        simplex0 = optimsimplex_destroy ( simplex0 )
        [ simplex0 , this ] = optimsimplex_new ( "randbounds" , xopt.' , ...
        costf_transposex , this.optbase.boundsmin , this.optbase.boundsmax , ...
        this.boxnbpointseff  , this )
    else
        errmsg = msprintf(gettext("%s: Unknown restart simplex method %s"), "neldermead_updatesimp", this.restartsimplexmethod)
        error(errmsg)
    end
    //
    // Scale the simplex into the bounds and the nonlinear inequality constraints, if any.
    // Caution !
    // The initial simplex may be computed with an "axis-by-axis" simplex,
    // so that it does not satisfies bounds constraints.
    // The scaling should therefore take into accounts for bounds.
    //
    nbve = optimsimplex_getnbve ( simplex0 );
    this = neldermead_log (this,"Before scaling:");
    str = string ( simplex0 )
    for i = 1:nbve
        this = neldermead_log (this,str(i));
    end
    [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
    if ( hasbounds | hasnlcons ) then
        this = neldermead_log (this,sprintf("Scaling initial simplex into nonlinear inequality constraints..."));
        nbve = optimsimplex_getnbve ( simplex0 )
        for ive = 1 : nbve
            // x is a row vector
            x = optimsimplex_getx ( simplex0 , ive );
            this = neldermead_log (this,sprintf("Scaling vertex #%d/%d at [%s]... " , ...
            ive , nbve , _strvec(x)));
            // Transpose x because xopt is a column vector : xp is now a column vector
            [ this , status , xp ] = _scaleinconstraints ( this , x.' , xopt );
            if ( ~status ) then
                errmsg = msprintf(gettext("%s: Impossible to scale the vertex #%d/%d at [%s] into inequality constraints"), "neldermead_updatesimp", ...
                ive , nbve , strcat(string(x)," "));
                error(errmsg);
            end
            if ( or(x <> xp) ) then
                index = 2
                if ( hasnlcons ) then
                    [ this.optbase , fv , c , index ] = optimbase_function ( this.optbase , xp , index );
                else
                    [ this.optbase , fv , index ] = optimbase_function ( this.optbase , xp , index );
                end
                // Transpose xp because optimsimplex takes row coordinate vectors.
                simplex0 = optimsimplex_setve ( simplex0 , ive , fv , xp.' )
            end
        end
    end
    this = neldermead_log (this,"After scaling:");
    str = string ( simplex0 )
    for i = 1:nbve
        this = neldermead_log (this,str(i));
    end
    this.simplex0 = optimsimplex_destroy ( this.simplex0 )
    this.simplex0 = simplex0;
    this.simplexsize0 = optimsimplex_size ( simplex0 );
    this.simplex0 = optimsimplex_sort ( this.simplex0 )
endfunction
//
// _strvec --
//  Returns a string for the given vector.
//
function str = _strvec ( x )
    str = strcat(string(x)," ")
endfunction
//
// costf_transposex --
//   Call the cost function and return the value.
//   Transpose the value of x, so that the input row vector,
//   given by optimsimplex, is transposed into a column vector,
//   as required by the cost function.
//
function [ f , this ] = costf_transposex ( x , this )
    xt = x.'
    [ f , this ] = neldermead_costf ( xt , this )
endfunction
//
// _scaleinconstraints --
//   Given a point to scale and a reference point which satisfies the constraints,
//   scale the point towards the reference point until it satisfies all the constraints.
//   Returns isscaled = %T if the procedure has succeded before -boxnbnlloops
//   Returns isscaled = %F if the procedure has failed after -boxnbnlloops
//   iterations.
// Arguments
//   x : the point to scale
//   xref : the reference point
//   isscaled : %T or %F
//   p : scaled point
//
function [ this , isscaled , p ] = _scaleinconstraints ( this , x , xref )
    p = x
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
    //
    // 1. No bounds, no nonlinear inequality constraints
    // => no problem
    //
    if ( ( hasbounds == %f ) & ( nbnlc == 0 ) ) then
        isscaled = %T
        return;
    end
    //
    // 2. Scale into bounds
    //
    if ( hasbounds ) then
        [ this.optbase , p ] = optimbase_proj2bnds ( this.optbase ,  p );
        this = neldermead_log (this,sprintf(" > After projection into bounds p = [%s]" , ..
        _strvec(p)));
    end
    //
    // 3. Scale into non linear constraints
    // Try the current point and see if the constraints are satisfied.
    // If not, move the point "halfway" to the centroid,
    // which should satisfy the constraints, if
    // the constraints are convex.
    // Perform this loop until the constraints are satisfied.
    // If all loops have been performed without success, the scaling
    // has failed.
    //
    isscaled = %F
    alpha = 1.0
    p0 = p
    while ( alpha > this.guinalphamin )
        [ this.optbase , feasible ] = optimbase_isinnonlincons ( this.optbase , p );
        if ( feasible ) then
            isscaled = %T;
            break;
        end
        alpha = alpha * this.boxineqscaling
        this = neldermead_log (this,sprintf("Scaling inequality constraint with alpha = %s", ..
        string(alpha)));
        p = ( 1.0 - alpha ) * xref + alpha * p0;
    end
    this = neldermead_log (this,sprintf(" > After scaling into inequality constraints p = [%s]" , ..
    _strvec(p) ) );
    if ( ~isscaled ) then
        this = neldermead_log (this,sprintf(" > Impossible to scale into constraints after %d loops" , ..
        this.optbase.nbineqconst ));
    end
endfunction

