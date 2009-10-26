// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimbase_histget --
//   Returns the history value for the given key.
//   If the key is unknown, generates an error.
// Arguments
//   iter : the iteration for which the data is to store
//   key : the name of the data to store
//
function value = optimbase_histget ( this , iter , key )
  if ( ~this.storehistory ) then
    errmsg = msprintf(gettext("%s: History disabled ; turn on -storehistory option.") , "optimbase_histget" )
    error(errmsg)
  end
  if iter < 1 then
    errmsg = msprintf(gettext("%s: Negative iteration index %d is not allowed.") , "optimbase_histget" , iter )
    error(errmsg)
  end
  select key
  case "-xopt" then
    value = this.historyxopt ( iter );
  case "-fopt" then
    value = this.historyfopt ( iter );
  else
    errmsg = msprintf(gettext("%s: Unknown key %s") , "optimbase_histget" ,key)
    error(errmsg)
  end
endfunction

