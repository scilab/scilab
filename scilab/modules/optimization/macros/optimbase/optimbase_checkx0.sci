// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_checkx0 --
//   Returns %T if the initial guess is consistent with the bounds
//   and the non linear inequality constraints, if any.
//
function this = optimbase_checkx0 ( this )
    [ this , isfeasible ] = optimbase_isfeasible ( this , this.x0 )
    if ( isfeasible <> 1 ) then
       error(sprintf ( gettext("%s: Initial guess is not feasible."),"optimbase_checkx0") )
    end
endfunction

