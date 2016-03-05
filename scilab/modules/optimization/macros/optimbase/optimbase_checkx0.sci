// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
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

