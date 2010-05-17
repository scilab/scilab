// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimbase_histset --
//   Set the history value at given iteration for the given key.
//   If the key is unknown, generates an error.
// Arguments
//   iter : the iteration for which the data is to store
//   key : the name of the data to store
//   value : the value to store
//
function this = optimbase_histset ( this , iter , key , value )
  if ( ~this.storehistory ) then
    errmsg = msprintf(gettext("%s: History disabled ; turn on -storehistory option.") , "optimbase_histset" )
    error(errmsg)
  end
  if iter < 1 then
    errmsg = msprintf(gettext("%s: Negative iteration index are not allowed.") , "optimbase_histset" )
    error(errmsg)
  end
  select key
  case "-xopt" then
    this.historyxopt ( iter ) = value;
  case "-fopt" then
    this.historyfopt ( iter ) = value;
  else
    errmsg = msprintf(gettext("%s: Unknown key %s" ) , "optimbase_histset" , key )
    error(errmsg)
  end
endfunction
