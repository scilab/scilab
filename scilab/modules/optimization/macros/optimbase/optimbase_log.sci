// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_log --
//   If verbose logging is enabled, prints the given message in the console.
//   If verbose logging is disabled, does nothing.
//
function this = optimbase_log (this,msg)
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_log", 2);
        error(errmsg)
    end
  if ( this.verbose == 1 ) then
    if ( this.logfile <> "" ) then
      mfprintf ( this.logfilehandle , "%s\n" , msg );
    else
      mprintf("%s\n",msg);
    end
  end
endfunction

