// =============================================================================
// Auhtor : Pierre MARECHAL
//          Scilab Team
// Copyright INRIA 2007
//
// Date   : 28/11/2007
//
// Determines if the path is an absolute name.
// =============================================================================

function status = is_absolute_path(pathname)
	
	status = %F;
	
	// Strips leading and trailing blanks and tabs of the path
	pathname = stripblanks(pathname,%T);
	
	if MSDOS then
		
		// Windows Case
		
		if (abs(str2code(part(pathname,1))) >= 10)    ..
			& (abs(str2code(part(pathname,1))) <= 35) ..
			& (part(pathname,2) == ":") then
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
