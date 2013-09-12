// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// optimsimplex_computefv --
//   Set the values of the function at vertices points.
// Arguments
//   fun : the function to compute at vertices
//   data : user-defined data
//
function [ this , data ] = optimsimplex_computefv ( varargin )
    function argin = argindefault ( rhs , vararglist , ivar , default )
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
    data = argindefault ( rhs , varargin , 3 , [] )
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

