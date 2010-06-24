// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
function varargout = loadwave(filename)

  lhs = argn(1);
  rhs = argn(2);
  
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),'loadwave',1));
    return
  end
  
  if lhs > 2 then
    error(msprintf(gettext("%s: Wrong number of output arguments.\n"),'loadwave'));
    return
  end
  
  if ~(type(filename) == 10) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n" ),'loadwave',1));
    return
  end
	
	if ~isfile(filename) then
	  error(msprintf(gettext("%s: Cannot open file %s.\n"),'loadwave',filename));
	end

  y = wavread(filename);
  
  if (lhs == 1) then
    varargout = list(y);
  elseif (lhs == 2) then
    info_wav = wavread(filename,'info');
    varargout = list(y, info_wav);
  else
    error(msprintf(gettext("%s: Wrong number of output argument(s).\n" ),'loadwave'));
    return
  end  
  
endfunction
// =============================================================================
