// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Remove a toolbox

function atomsDisp(str)
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsDisp",1,2))
	end
	
	if type(str) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsDisp",1));
	end
	
	// Verbose Mode ?
	// =========================================================================
	
	if isdef("ATOMSVERBOSE") then
		if ATOMSVERBOSE then
			mprintf(str);
			if size(str,"*")>1 then
				mprintf("\n");
			end
		end
	elseif strcmpi(atomsGetConfig("Verbose"),"True") == 0
		mprintf(str);
		if size(str,"*")>1 then
			mprintf("\n");
		end
	end
	
endfunction
