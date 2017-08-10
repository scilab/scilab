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
// nmplot_function --
//   Call the cost function and return the value.
// Arguments
//   x : the point where the function is to be evaluated.
//   index : a flag to pass to the cost function (default = 1)
//   f : the cost function
//
function [ this , f ] = nmplot_function ( this , x )
    [ this.nmbase , f ] = neldermead_function ( this.nmbase , x )
endfunction
