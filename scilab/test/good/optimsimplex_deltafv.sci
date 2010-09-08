// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_deltafv --
//   Returns the vector of difference of function
//   values with respect to the function value at vertex #1.
// Arguments
//   <no arg>
//
function df = optimsimplex_deltafv ( this )
  df = this.fv(2:this.nbve) - this.fv(1)*ones(this.nbve-1,1)
endfunction

