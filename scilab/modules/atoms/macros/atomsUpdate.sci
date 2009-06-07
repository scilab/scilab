// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// update of a toolbox

function result = atomsUpdate(name,allusers)
	
	result = [];
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsUpdate",0,1))
	end
	
	if (rhs>0) & (type(name)<>10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsUpdate",1));
	end
	
	name = stripblanks(name);
	
	// Install for all users or just for me ?
	// =========================================================================
	
	if rhs == 1 then
		// By default, install for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstall",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Check if all specified toolboxes are effectively installed
	// =========================================================================
	
	for i=1:size(name,"*")
		if ~ atomsIsInstalled(name(i)) then
			error(msprintf(gettext("%s: ''%s'' isn''t installed.\n"),"atomsUpdate",name(i)));
		end
	end
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion('scilab')) + ".");
	
	// List all installed packages (needed for later)
	// =========================================================================
	package_installed = atomsGetInstalled();
	
	// Update all the Toolboxes
	// =========================================================================
	
	if rhs==0 | name == "all" | isempty(name) then
		package_installed = atomsGetInstalled();
		name              = package_installed(:,1);
	end
	
	// Loop on name
	// =========================================================================
	
	for i=1:size(name,"*")
		
		this_package_versions = atomsGetInstalledVers(name(i),allusers);
		this_package_MRV_ins  = this_package_versions(1);   // Most Recent Version Installed
		this_package_MRV_ava  = atomsGetMRVersion(name(i)); // Most Recent Version Available
		
		if atomsVersionCompare(this_package_MRV_ins,this_package_MRV_ava) == 0 then
			// The installed version is already the Most Recent Version Available
			continue;
		end
		
		atomsInstall(name(i)+" "+this_package_MRV_ava,allusers);
		
	end
	
endfunction
