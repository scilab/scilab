
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function help(key)

if (fileinfo('SCI/modules/helptools/help') <> []) then
  
  if getscilabmode() <> "NWNI" then
    
    // No input argument: launch help browser
    if argn(2)==0 then
      global %helps
      helpbrowser(%helps(:,1), getlanguage());
      return
    end
    
    // Search a function name
    key=stripblanks(key)
    
    if or(part(key,1)==['(',')','[',']','{','}','%','''','""',':','*','/', ...
	    '\','.','<','>','&','^','|','~','+','-']) then
      key="symbols";
    end
  
    global %helps
    helpbrowser(%helps(:,1), key, getlanguage(), %f);
    
    // If the function name does not exists then full-text search is done (See Java code)
    
  else

    error(msprintf(gettext("%s: The help browser is disabled in %s mode.\n"), "help", getscilabmode()));
  
  end
else

  error(msprintf(gettext("%s: help module is not installed.\n"), "help"));

end

endfunction









