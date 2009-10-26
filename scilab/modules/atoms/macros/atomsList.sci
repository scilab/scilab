// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// display of the available toolboxes

function atomsList()
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 0 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsInstall",0))
	end
	
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
		this_package_details = atomsToolboxDetails([this_package_name,this_package_version]);
		this_package_summary = this_package_details("Summary");
		
		packages_disp        = [ packages_disp ; this_package_name this_package_summary ];
		
	end
	
	// Loop on package list ... a second time
	// =========================================================================
	
	for i=1:size(packages_disp(:,1),"*")
		mprintf("% "+string(max(length(packages_disp(:,1))))+"s - %s\n",packages_disp(i,1),packages_disp(i,2));
	end
	
endfunction
