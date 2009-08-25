// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Search

function packages_disp = atomsSearch( search , fields )
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsSearch",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(search) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsSearch",1));
	end
	
	if (rhs>1) & (type(fields)<>10)  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsSearch",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(search,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsSearch",1));
	end
	
	// Don't take into account the character cast
	// =========================================================================
	search = convstr(search,"l");
	
	// Get the list of available toolboxes
	// =========================================================================
	
	packages_struct    = atomsGetTOOLBOXES();
	
	packages_list      = getfield(1,packages_struct);
	packages_list(1:2) = [];
	
	packages_disp      = [];
	
	// Loop on package list
	// =========================================================================
	
	for i=1:size(packages_list,"*")
		
		this_package_name = packages_list(i);
		
		// Get the list of versions compatibles with this version of Scilab
		this_package_versions = atomsCompatibleVersions(packages_list(i));
		
		if isempty(this_package_versions) then
			continue;
		end
		
		// Use the Most Recent Version
		this_package_version = this_package_versions(1);
		
		// Get the details of this toolbox
		this_package_details = atomsToolboxDetails(this_package_name,this_package_version);
		this_package_summary = this_package_details("Summary");
		
		if grep( convstr(this_package_name,"l") , search ) <> [] then
			packages_disp = [ packages_disp ; this_package_name this_package_summary ];
		elseif grep( convstr(this_package_summary,"l") , search ) <> [] then
			packages_disp = [ packages_disp ; this_package_name this_package_summary ];
		end
		
	end
	
endfunction
