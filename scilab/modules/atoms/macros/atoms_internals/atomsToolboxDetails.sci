// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Return the mlist associated with the wanted toolbox

function details = atomsToolboxDetails(name,version,field)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsToolboxDetails",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",2));
	end
	
	if rhs>2 & type(field)<>10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",3));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",1));
	end
	
	if size(version,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",2));
	end
	
	if rhs>2 & size(field,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsToolboxDetails",3));
	end
	
	// Get the list of installed packages
	// =========================================================================
	packages = atomsGetTOOLBOXES();
	
	if ~ isfield(packages,name) then
		error(msprintf(gettext("%s: the package ''%s'' doesn''t exist.\n"),"atomsToolboxDetails",name));
	end
	
	package_versions = packages(name);
	
	if ~ isfield(package_versions,version) then
		error(msprintf(gettext("%s: the package ''%s'' doesn''t exist.\n"),"atomsToolboxDetails",name+" - "+version));
	end
	
	details = package_versions(version);
	
	// A field is gived :
	// =========================================================================
	
	if rhs>2 then
		
		if ~ isfield(details,field) then
			error(msprintf(gettext("%s: the package ''%s'' doesn''t contain the field ''%s''.\n"),"atomsToolboxDetails",name+" - "+version,field));
		end
		
		details = details(field);
		
		return;
	end
	
	// Check if this version is installed
	// =========================================================================
	
	if atomsIsInstalled(name,version) then
		
		installedDetails = atomsGetInstalledDetails(name,version);
		details("installAlluser") = installedDetails(3);
		details("installPath")    = installedDetails(4);
		details("installStatus")  = installedDetails(5);
		
	end
	
endfunction
