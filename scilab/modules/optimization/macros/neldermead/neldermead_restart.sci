// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_restart --
//   Update the simplex and restart the search.
//
function this = neldermead_restart ( this )
  this = neldermead_updatesimp ( this );
  this = neldermead_search (this);
endfunction

