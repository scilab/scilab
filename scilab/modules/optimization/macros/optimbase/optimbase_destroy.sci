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
// optimbase_destroy --
//   Destroy an object.
//
function this = optimbase_destroy (this)
    [lhs,rhs]=argn();
    if ( rhs <> 1 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_destroy", 1);
        error(errmsg)
    end
  this.historyfopt = [];
  this.historyxopt = [];
  if ( this.logstartup ) then
    this = optimbase_logshutdown ( this );
  end
endfunction

