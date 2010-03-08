// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// isnum(str) return %T if str represent a number
//
// =============================================================================

function res = isnum(str)
	
	if type(str) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of character strings.\n"),"isnum",1));
	end
	
	[nr,nc] = size(str);
	
	for j=1:nc
		for i=1:nr
		  ierr = execstr('r = type(eval(str(i,j)))','errcatch');
		  if ierr == 0 then
		    if r == 1 then
		      res(i,j) = %T;
		    else
		      res(i,j) = %F;
		    end
		  else
		    res(i,j) = %F;
		  end
		end
	end
	
endfunction

