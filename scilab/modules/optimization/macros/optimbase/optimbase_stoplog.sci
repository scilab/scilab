// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_stoplog --
//   Prints the given stopping rule message if verbose termination is enabled.
//   If verbose termination is disabled, does nothing.
//
function this = optimbase_stoplog ( this , msg )
  if ( this.verbose == 1 ) then
  if ( this.verbosetermination == 1 ) then
    this = optimbase_log ( this , msg )
  end
  end
endfunction

