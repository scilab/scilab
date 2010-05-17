// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



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
