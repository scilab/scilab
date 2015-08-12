// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

