// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Tri un tableau de versions

function versions_out = atomsVersionSort( versions_in , direction )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsVersionSort",2));
	end
	
	// if versions_in == [], no need to continue
	// =========================================================================
	
	if versions_in == [] then
		versions_out = [];
		return;
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(versions_in) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsVersionSort",1));
	end
	
	if type(direction) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsVersionSort",2));
	end
	
	// Check input parameters dimension
	// =========================================================================
	
	if size(direction,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsVersionSort",2));
	end
	
	// Check input parameters value
	// =========================================================================
	if regexp(direction,"/^(ASC|DESC)$/i","o") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ""ASC"" or ""DESC"" expected.\n"),"atomsVersionSort",2));
	end
	
	// Output argument
	// =========================================================================
	versions_out = versions_in;
	
	ASC  = (convstr(direction,"u") == "ASC");
	DESC = ~ ASC;
	
	// Now : action
	// =========================================================================
	
	for i=1:(size(versions_out,"*")-1)
		for j=1:(size(versions_out,"*")-1)
			if ASC & (atomsVersionCompare(versions_out(j),versions_out(j+1)) > 0) then
				tmp               = versions_out(j+1);
				versions_out(j+1) = versions_out(j);
				versions_out(j)   = tmp;
			elseif DESC & (atomsVersionCompare(versions_out(j),versions_out(j+1)) < 0) then
				tmp               = versions_out(j+1);
				versions_out(j+1) = versions_out(j);
				versions_out(j)   = tmp;
			end
		end
	end
	
	// Return a colum vector
	// =========================================================================
	versions_out = matrix(versions_out,-1,1);
	
endfunction
