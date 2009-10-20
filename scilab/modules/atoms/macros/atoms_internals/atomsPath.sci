// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function path = atomsPath(whataction,section)
	
	path = [];
	
	// Check number of input arguments
	// =========================================================================
	rhs = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsPath",2));
	end
	
	// Check input argument type
	// =========================================================================
	
	if type(whataction) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single String expected.\n"),"atomsPath",1));
	end
	
	if type(section) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single String expected.\n"),"atomsPath",2));
	end
	
	// Check input argument value
	// =========================================================================
	
	if and(whataction<>["system","install"]) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''system'' or ''install'' expected.\n"),"atomsPath",1));
	end
	
	if and(section<>["all","allusers","user","session"]) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''all'',''allusers'',''user'' or ''session'' expected.\n"),"atomsPath",2));
	end
	
	// Check input argument value
	// =========================================================================
	
	if (whataction=="system") then
		
		if or( section == ["all","allusers"]) then
			path = [ path ; pathconvert(SCI+"/.atoms") ];
		end
		
		if or( section == ["all","user"]) then
			path = [ path ; pathconvert(SCIHOME+"/.atoms") ];
		end
		
		if or( section == ["session"]) then
			path = [ path ; pathconvert(TMPDIR+"/.atoms") ];
		end
		
	elseif (whataction=="install") then
		
		if or( section == ["all","allusers"]) then
			path = [ path ; pathconvert(SCI+"/contrib") ];
		end
		
		if or( section == ["all","user"]) then
			path = [ path ; pathconvert(SCIHOME+"/atoms") ];
		end
		
	end
	
endfunction
