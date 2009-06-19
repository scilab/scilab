// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Renvoie les détails sur l'installation d'un package.

function res = atomsGetInstalledDetails(name,version)
	
	rhs = argn(2);
	res = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledStatus",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsGetInstalledStatus",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsGetInstalledStatus",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if or( size(name,"*") <> size(version,"*"))  then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsGetInstalledStatus",1,2));
	end
	
	// Get the list of installed packages
	// =========================================================================
	packages = atomsGetInstalled();
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		// Filter on names
		packages_filtered = packages( find(packages(:,1) == name(i)) , : );
		
		// Filter on versions
		packages_filtered = packages_filtered( find(packages_filtered(:,2) == version(i)) , : );
		
		if ~ isempty(packages_filtered) then
			res = [ res ; packages_filtered ];
		else
			res = [ res ; "" "" "" "" "" ];
		end
		
	end
	
endfunction
