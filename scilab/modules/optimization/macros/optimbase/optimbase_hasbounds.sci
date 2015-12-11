// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt



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

