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
// optimbase_hasconstraints --
//   Returns %T if current problem has
//   * bounds constraints or
//   * linear constraints or
//   * non linear constraints.
// Arguments
//   this : the current object
//   hasbounds : %T or %F (see above)
// TODO  : linear constraints
//
function [ this , hascons ] = optimbase_hasconstraints ( this )
    hascons = %f
    hascons = ( hascons | ( this.nbineqconst > 0 ) )
    maxl = length ( this.boundsmax )
    minl = length ( this.boundsmin )
    hascons = ( hascons | ( maxl <> 0 ) | ( minl <> 0 ) )
endfunction
