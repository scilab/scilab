// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) - 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [path, fname, extension] = fileparts(inname, sel)

	[lhs, rhs] = argn(0);
	
	if (rhs == 2) then
	  if type(sel) <> 10 then
	    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"fileparts",2));
	  end
	end
	
	if type(inname) <> 10 then
	  error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"fileparts",1));
	end
	
  path = '';
  fname = '';
  extension = '';
  
  WIN_DRIVE_SEP = ':';
  
  name = stripblanks(inname);
  if stripblanks(inname) == '' then 
    return
  end

  inname = pathconvert(inname, %f, %f);

  seps = filesep();
  if MSDOS then 
    seps = [seps ,WIN_DRIVE_SEP];
  end

  ind = strindex(inname, seps);
  if (ind == []) then
    fname = inname;
  else
    if part(inname,ind($)) == WIN_DRIVE_SEP then
      path = part(inname,1:ind($))+seps(1);
    else
      path = part(inname,1:ind($));
    end
    fname = part(inname,ind($)+1:length(inname));
  end

  ind = strindex(fname,'.');
  if ind <> [] then
    if (size(ind,'*') == 1) & (ind == 1) then
      extension = '';
      fname = part(fname, ind($):length(fname));
    else
      extension = part(fname, ind($):length(fname));
      fname = part(fname, 1:ind($)-1);
    end
  end
  
  if (rhs == 2) & (lhs == 1) then
    if (sel == 'path') | (sel == 'fname') | (sel == 'extension') then
      execstr('path=' + sel);
      execstr('extension=""""');
      execstr('fname=""""');
    else
      error(999, msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' or ''%s'' expected.\n"),"fileparts",2,"path","fname","extension"));
	    return;
    end
  end
    
endfunction
