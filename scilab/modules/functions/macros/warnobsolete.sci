// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This function is to warn user that he's using a deprecated function

function warnobsolete(newFunctionName, scilabRemovedVersion)
  // Modal warning is to set if this function
  // should block execution or only print
  // on the standard outstream
  global %modalWarning
  
  //Retrieve Calling function
  [lineCall, stackCall]=where()
  
  // Now build the correct warning message
  warnMessage = msprintf(_("Function %s is obsolete."),stackCall(2))
  if argn(2) >= 1 then
    warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newFunctionName)]
  end
  if argn(2) >= 2 then
    warnMessage = [warnMessage, msprintf(_("This function will be permanently removed in Scilab %s"), scilabRemovedVersion)]
  end
  
  // Now show the warning
  
  if %modalWarning then
    x_message(warnMessage,"modal");
  else
    warning(warnMessage);
  end
  
  clear %modalWarning
  
endfunction
