// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_new --
//   Creates a new Nelder - Mead plot object.
//
function newobj = nmplot_new ()
  newobj = tlist(["T_NELDERMEADPLOT",...
    "simplexfn","fbarfn","foptfn","sigmafn","nmbase",...
    "simplexhandle","fbarhandle","fopthandle","sigmahandle"]);
  newobj.simplexfn = "";
  newobj.fbarfn = "";
  newobj.foptfn = "";
  newobj.sigmafn = "";
  newobj.simplexhandle = 0;
  newobj.fbarhandle = 0;
  newobj.fopthandle = 0;
  newobj.sigmahandle = 0;
  //
  // Create and configure parent class
  //
  newobj.nmbase = neldermead_new();
  newobj.nmbase = neldermead_configure ( newobj.nmbase , "-outputcommand" , nmplot_outputcmd );
endfunction

