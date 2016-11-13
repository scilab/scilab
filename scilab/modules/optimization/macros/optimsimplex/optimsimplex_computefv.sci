// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
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
// optimsimplex_computefv --
//   Set the values of the function at vertices points.
// Arguments
//   fun : the function to compute at vertices
//   data : user-defined data
//
function [ this , data ] = optimsimplex_computefv ( varargin )
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
    if (rhs>3) then
        errmsg = msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "optimsimplex_computefv", 2,3);
        error(errmsg)
    end
    this = varargin(1)
    fun = varargin(2)
    data = optim_argindefault ( rhs , varargin , 3 , [] )
    if (typeof(data)=="constant") then
        if ( data == [] ) then
            this = optimsimplex_compsomefv ( this , fun , 1:this.nbve )
        else
            [ this , data ] = optimsimplex_compsomefv ( this , fun , 1:this.nbve , data )
        end
    else
        [ this , data ] = optimsimplex_compsomefv ( this , fun , 1:this.nbve , data )
    end
endfunction

