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
// optimbase_hasbounds --
//   Returns %T if current problem has bounds.
// Arguments
//   this : the current object
//   hasbounds : %T or %F (see above)
//
function [ this , hasbounds ] = optimbase_hasbounds ( this )
    maxl = length ( this.boundsmax )
    minl = length ( this.boundsmin )
    hasbounds = ( maxl <> 0 | minl <> 0 )
endfunction

