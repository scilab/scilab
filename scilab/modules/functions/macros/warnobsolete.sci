//
//  -*- Scilab -*-
//
// warnobsolete.sci
// Made by  Bruno JOFRET <bruno.jofret@inria.fr>
//
// Started on  Thu Feb 14 10:33:50 2008 bruno
// Last update Thu Feb 14 11:06:16 2008 bruno
//
// Copyright INRIA 2008
//

// This function is to warn user that he's using a deprecated function

function warnobsolete(newFunctionName, scilabRemovedVersion)
  // Modal warning is to set if this function
  // should block execution or only print
  // on the standard outstream
  global %modalWarning
  if %modalWarning then
    warnUser = x_message
  else
    warnUser = warning
  end

  //Retrieve Calling function
  [lineCall, stackCall]=where()
  
  // Now build the correct warning message
  warnMessage = msprintf(_("Function %s is obsolete."),stackCall(2))
  if argn(2) >= 1 then
    warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newFunctionName)]
  end
  if argn(2) >= 2 then
    warnMessage = [warnMessage, msprintf(_("This function will be definitelly removed in Scilab %s"), scilabRemovedVersion)]
  end

  // Now show the warning
  warnUser(warnMessage);
endfunction
