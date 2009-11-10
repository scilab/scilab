// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Output argument :

//   path : . If the toolbox is loaded, path is the path of the loaded toolbox
//            otherwise, path = ""
//          . Matrix of string (n x 1)
//          . mandatory

function path = atomsTest(packages)
	
	rhs         = argn(2);
	path        = [];
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsTest",1))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsTest",1));
	end
	
	if size(packages(1,:),"*") > 3 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1,mx2 or mx3 string matrix expected.\n"),"atomsTest",1));
	end
	
	packages = stripblanks(packages);
	
	// Complete packages matrix with empty columns
	// =========================================================================
	
	if size(packages(1,:),"*") == 1 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];
	
	elseif size(packages(1,:),"*") == 2 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	
	end
	
	// Check if the module is installed
	// =========================================================================
	
	if ~ and(atomsIsInstalled(packages)) then
		for i=1:size(packages(:,1),"*")
			if ~ atomsIsInstalled(packages(i,:)) then
				if isempty(packages(i,2)) & isempty(packages(i,3)) then
					error(msprintf(gettext("%s: Module ''%s'' is not installed.\n"),"atomsTest",packages(i,1)));
				elseif ~isempty(packages(i,2)) & isempty(packages(i,3)) then
					error(msprintf(gettext("%s: ''%s - %s'' is not installed.\n"),"atomsTest",packages(i,1),packages(i,2)));
				elseif isempty(packages(i,2)) & ~isempty(packages(i,3)) then
					error(msprintf(gettext("%s: ''%s'' (''%s'' section) is not installed.\n"),"atomsTest",packages(i,1),packages(i,3)));
				elseif ~isempty(packages(i,2)) & ~isempty(packages(i,3)) then
					error(msprintf(gettext("%s: ''%s - %s'' (''%s'' section) is not installed.\n"),"atomsTest",packages(i,1),packages(i,2),packages(i,3)));
				end
			end
		end
	end
	
	// Add a fourth column : the module installation path
	// =========================================================================
	packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	
	// Loop on input parameter to set the version and the section if it's not 
	// already done
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		// The module's installed version hasn't been specified or is empty
		// → Set the MRV available
		// =====================================================================
		
		if isempty(packages(i,2)) then
			
			if ~ isempty(packages(i,3)) then
				section = packages(i,3);
			
			else
				section = "all";
			
			end
			
			this_module_versions = atomsGetInstalledVers(packages(i,1),section);
			
			if isempty(this_module_versions) then
				if section == "all" then
					error(msprintf(gettext("%s: Module ''%s'' is not installed.\n"),"atomsTest",packages(i,1)));
				else
					error(msprintf(gettext("%s: Module ''%s'' is not installed (''%s'' section).\n"),"atomsTest",packages(i,1),section));
				end
			else
				packages(i,2) = this_module_versions(1);
			end
			
		else
			
			if ~atomsIsInstalled([packages(i,1) packages(i,2)]) then
				error(msprintf(gettext("%s: Module ''%s - %s'' is not installed.\n"),"atomsLoad",packages(i,1),packages(i,2)));
			end
			
		end
		
		// The module's installed section hasn't been specified or is empty
		// → If the module (same name/same version) is installed in both sections,
		//   module installed in the "user" section is taken
		// =====================================================================
		
		if isempty(packages(i,3)) then
			
			sections = ["user","allusers"];
			
			for j=1:size(sections,"*")
				if atomsIsInstalled([packages(i,1) packages(i,2)],sections(j)) then
					packages(i,3) = sections(j);
				end
			end
			
		else
		
			// Check if modules are installed
			if ~ atomsIsInstalled([packages(i,1) packages(i,2)],packages(i,3)) then
				mprintf(gettext("%s: The following modules are not installed:\n"),"atomsAutoloadAdd");
				mprintf("\t - ''%s - %s'' (''%s'' section)\n",packages(i,1),packages(i,2),packages(i,3));
				error("");
			end
			
		end
		
		// Get the installed path
		// =====================================================================
		packages(i,4) = atomsGetInstalledPath([packages(i,1) packages(i,2) packages(i,3)]);
		
	end
	
	// Loop on packages
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		test_run(packages(i,4));
	end
	
endfunction
