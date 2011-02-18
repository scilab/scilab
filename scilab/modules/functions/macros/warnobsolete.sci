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
  if size(stackCall,"*") < 2 then
    error(msprintf(gettext("%s: Private function: cannot access to this function directly.\n"),"warnobsolete"));
  end
  // Now build the correct warning message
  warnMessage = msprintf(_("Feature %s is obsolete."),stackCall(2))
  if exists("newFunctionName", 'l') then
    warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newFunctionName)]
  end
  if exists("scilabRemovedVersion", 'l') then
    warnMessage = [warnMessage, msprintf(_("This feature will be permanently removed in Scilab %s"), scilabRemovedVersion)]
  end
  
  // Now show the warning
  
  if %modalWarning then
    messagebox(warnMessage,"modal");
  else
    warning(warnMessage);
  end
  
  clear %modalWarning
  
endfunction
