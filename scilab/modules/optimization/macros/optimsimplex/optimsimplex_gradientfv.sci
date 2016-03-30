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
// optimsimplex_gradientfv --
//   Returns the simplex gradient of the function.
// Arguments
//   fun : the function to use
//   method : the method to use, "forward" or "centered"
//   data : user-defined data, added to the function if provided
//
function [ g , data ] = optimsimplex_gradientfv ( this , fun , method , data )
    if ( this.nbve <> this.n+1 ) then
        errmsg = msprintf(gettext("%s: The gradient can be applied only with a simplex made of n+1 points, but the dimension is %d and the number of vertices is %d"), "optimsimplex_gradientfv" , this.n , this.nbve)
        error(errmsg)
    end
    if (~isdef("method","local")) then
        method = "forward";
    end
    select method
    case "forward" then
        g = optimsimplex_gradforward ( this )
    case "centered" then
        if (~isdef("data","local")) then
            g = optimsimplex_gradcenter ( this , fun )
        else
            [ g , data ] = optimsimplex_gradcenter ( this , fun , data )
        end
    else
        errmsg = msprintf(gettext("%s: Unknown method %s"),"optimsimplex_gradientfv",method)
        error(errmsg)
    end
endfunction
//
// optimsimplex_gradforward --
//   Returns the simplex forward gradient of the function.
// Arguments
//   <no arg>
//
function g = optimsimplex_gradforward ( this )
    v = optimsimplex_dirmat ( this )
    d = optimsimplex_deltafv ( this )
    g = v.'\d
endfunction
//
// optimsimplex_gradcenter --
//   Returns the simplex centered gradient of the function.
// Arguments
//   fun : name of the function
//   data : user-defined data
//
function [ g , data ] = optimsimplex_gradcenter ( this , fun , data )
    g1 = optimsimplex_gradforward ( this )
    if (~isdef("data","local")) then
        r = optimsimplex_reflect ( this , fun )
    else
        [ r , data ] = optimsimplex_reflect ( this , fun , data )
    end
    g2 = optimsimplex_gradforward ( r )
    g = (g1 + g2)/2
    r = optimsimplex_destroy ( r )
endfunction

