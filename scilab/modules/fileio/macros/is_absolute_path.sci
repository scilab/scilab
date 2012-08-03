// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
// Date   : 28/11/2007
//
// Determines if the path is an absolute name.
// =============================================================================

function status = is_absolute_path(pathname)
	
	status = %F;
	
	// Strips leading and trailing blanks and tabs of the path
	pathname = stripblanks(pathname,%T);
	
	if getos() == 'Windows' then
		
		// Windows Case
		
		if isletter(part(pathname,1)) & (part(pathname,2) == ":") then
			status = %T;
		end
		
		if (part(pathname,[1:7])=="file:")   ..
			| (part(pathname,[1:2])=="\\")   ..
			| (part(pathname,[1:2])=="//") then
			status = %T;
			
		end
		
	else
		
		// Unix case
		
		status = (part(pathname,1)=="/") | (part(pathname,1)=="~");
	end
	
endfunction
