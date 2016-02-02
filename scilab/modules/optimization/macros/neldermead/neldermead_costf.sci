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
// neldermead_costf --
//   Call the cost function and return the value.
// Note
//   This function is given to the simplex class as
//   a callback.
//   Input/Output arguments are swapped w.r.t.
//   optimbase_function, so that it matches
//   the requirements of simplex methods.
//
function [ f , this ] = neldermead_costf ( x , this )
    [ this.optbase , hascons ] = optimbase_hasnlcons ( this.optbase );
    if ( hascons ) then
        [ this.optbase , f , c , index ] = optimbase_function ( this.optbase , x , 2 );
    else
        [ this.optbase , f , index ] = optimbase_function ( this.optbase , x , 2 );
    end
endfunction

