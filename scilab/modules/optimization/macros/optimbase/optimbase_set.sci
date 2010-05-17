// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimbase_set --
//   Set the value for the given key.
//   If the key is unknown, generates an error.
//
function this = optimbase_set ( this , key , value )
  select key
  case "-iterations" then
    this.iterations = value;
  case "-xopt" then
    this.xopt = value;
  case "-fopt" then
    this.fopt = value;
  case "-historyxopt" then
    if ( ~this.storehistory ) then
      errmsg = msprintf( gettext ( "%s: History disabled ; turn on -storehistory option.") , "optimbase_set" )
      error(errmsg)
    else
      this.historyxopt = value;
    end
  case "-historyfopt" then
    if ( ~this.storehistory ) then
      errmsg = msprintf(gettext ( "%s: History disabled ; turn on -storehistory option.") , "optimbase_set" )
      error(errmsg)
    else
      this.historyfopt = value;
    end
  case "-fx0" then
    this.fx0 = value;
  case "-status" then
    this.status = value;
  else
    errmsg = msprintf(gettext ( "%s: Unknown key %s") , "optimbase_set", key )
    error(errmsg)
  end
endfunction

