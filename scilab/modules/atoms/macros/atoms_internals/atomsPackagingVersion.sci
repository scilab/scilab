// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return true if the package is a valid package or not

function result = atomsPackagingVersion(packages)
	
	// Initialize
	// =========================================================================
	result = [];
	
	// Check input parameters
	// =========================================================================
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsPackagingVersion",1));
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsPackagingVersion",1));
	end
	
	if size(packages(1,:),"*") <> 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 string matrix expected.\n"),"atomsPackagingVersion",1));
	end
	
	// Get all package description
	// =========================================================================
	allpackages = atomsDESCRIPTIONget();
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		result(i) = "",
		name      = packages(i,1);
		version   = packages(i,2);
		
		// 1st case : no version is mentioned
		if isempty(version) then
			result(i) = "";
		
		// 2nd case : the packaging version is already mentioned
		elseif ~ isempty(strindex(version,"-")) then
			result(i) = version;
		
		// 3rd case : the packaging version is not mentioned
		else
			
			// Loop on this packages versions
			package_versions          = allpackages(name);
			package_versions_tab      = getfield(1,package_versions);
			package_versions_tab(1:2) = [];
			
			for j=1:size(package_versions_tab,"*")
				
				if isempty( strindex(package_versions_tab(j),"-") ) then
					if package_versions_tab(j) == version then
						result(i) = version;
						break;
					end
				
				else
					if part(package_versions_tab(j), 1:strindex(package_versions_tab(j),"-")-1)==version then
						result(i) = package_versions_tab(j);
						break;
					end
					
				end
				
			end
		end
		
	end
	
endfunction
