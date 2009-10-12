// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



//
// optimbase_hasnlcons --
//   Returns %T if current problem has non 
//   linear constraints.
// Arguments
//   this : the current object
//   hasnlcons : %T or %F (see above)
//
function [ this , hasnlcons ] = optimbase_hasnlcons ( this )
  hasnlcons = ( this.nbineqconst > 0 )
endfunction

