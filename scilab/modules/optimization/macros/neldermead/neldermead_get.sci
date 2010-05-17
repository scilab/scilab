// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// neldermead_get --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//
function value = neldermead_get ( this , key )
  select key
  case "-historysimplex" then
    storehistory = optimbase_cget ( this.optbase , "-storehistory" );
    if ( ~storehistory ) then
      errmsg = msprintf(gettext("%s: History disabled ; turn on -storehistory option."), "neldermead_get")
      error(errmsg)
    else
      value = this.historysimplex;
    end
  case "-simplexopt" then
    value = this.simplexopt;
  case "-simplex0" then
    value = this.simplex0;
  case "-restartnb" then
    value = this.restartnb;
  else
    // Delegate to optbase
    value = optimbase_get ( this.optbase , key );
  end
endfunction

