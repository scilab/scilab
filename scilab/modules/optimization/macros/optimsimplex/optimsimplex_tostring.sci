// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_tostring --
//   Returns the current simplex as a string.
// Arguments
//   <no arg>
//
function str = optimsimplex_tostring ( this )
  warnobsolete("string", "optimsimplex_tostring")
  str = string(this)
endfunction
