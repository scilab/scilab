// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function varargout = fscanf(fil, frmt)

// fscanf - Emulator of C language fscanf
//!

	[lhs, rhs] = argn(0);
	
	MAXLHS = 50;
	if lhs > MAXLHS then
	  error(999, msprintf(gettext("%s: Wrong number of output argument(s).\n"),"fscanf"));
	end
	
	if type(fil)<>10 & type(fil)<> 1 then
		error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A file descriptor expected.\n"),"fscanf",1));
	end
	
	if size(fil,"*")<>1 then
		error(999, msprintf(gettext("%s: Wrong size for input argument #%d: A file descriptor expected.\n")),"fscanf",1);
	end
	
	if type(frmt)<>10 then
		error(999, msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"fscanf",2));
	end
	
	if size(frmt,"*")<>1 then
		error(999, msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n")),"fscanf",2);
	end
	
	
	if type(fil) == 1 then 
	  fids = file();
	  if ~or(fids == fil) then
	    error(999, msprintf(gettext("%s: Wrong value for input argument #%d: A valid file descriptor expected.\n"),"fscanf", 1));
	    return;
	  end
	end
	
  v = 'v';
  args = strcat( v(ones(lhs,1)) + string(1:lhs)',',');
  buf = read(fil, 1, 1, '(a)');
  execstr('[' + args + '] = sscanf(buf,frmt);');
  execstr('varargout = list(' + args + ');');
  
endfunction
