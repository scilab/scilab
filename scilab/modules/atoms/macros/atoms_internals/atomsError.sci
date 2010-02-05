// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function atomsError(issue,msg)
	
	// Check input argments
	// =========================================================================
	
	rhs = argn(2);
	
	// Number of input argments
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsError",2))
	end
	
	// 1st input argument
	
	if type(issue) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsError",1));
	end
	
	if size(issue,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single-string expected.\n"),"atomsError",1));
	end
	
	if and( issue<>["error","warning"] ) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d:''error'' or ''warning'' expected.\n"),"atomsError",1));
	end
	
	// 2nd input argument
	
	if type(msg) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsError",2));
	end
	
	// Display the error message
	// =========================================================================
	
	for i=1:size(msg,"*")
		mprintf(msg(i)+"\n");
	end
	
	// Go to the initial path if defined
	// =========================================================================
	if isdef("ATOMSINITIALPATH") & ~ isempty(ATOMSINITIALPATH) & isdir(ATOMSINITIALPATH) then
		chdir(ATOMSINITIALPATH);
	end
	
	// Throw error or warning
	// =========================================================================
	
	if issue == "error" then
		error("");
	
	elseif issue == "warning" then
		warning("");
	
	end
	
	
endfunction
