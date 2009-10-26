// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add toolboxes to the list of packages that are automaticaly loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/autoloaded

// End-User function

function nbAdd = atomsAutoloadAdd(packages,section)
	
	rhs            = argn(2);
	nbAdd          = 0;
	autoloaded     = []; // Column vector that contain autoloaded packages
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check write access on allusers zone
	// =========================================================================
	ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadAdd",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadAdd",1));
	end
	
	if (size(packages(1,:),"*") > 3) | (size(packages(1,:),"*") < 2) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 or mx3 string matrix expected.\n"),"atomsAutoloadAdd",1));
	end
	
	// Allusers/user management
	//   - If section is equal to "allusers", module(s) will added to the 
	//     "autoload" list of the "allusers" section :
	//       → SCI/.atoms/autoloaded
	//   - Otherwise, module(s) will added to the "autoload" list of the "user"
	//     section
	//       → SCIHOME/atoms/autoloaded
	// =========================================================================
	
	if rhs < 2 then
		
		// By default: 
		//  → Add the module to the "autoload" list of the "allusers" section
		//    if we have the write access to SCI directory
		//  → Add the module to the "autoload" list of the "user" section otherwise
		
		if ATOMSALLUSERSWRITEACCESS then
			section = "allusers"; 
		else
			section = "user";
		end
		
	else
		
		// Process the 2nd input argument : allusers
		// Allusers can be equal to "user" or "allusers"
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsAutoloadAdd",2));
		end
		
		if and(section<>["user","allusers"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsAutoloadAdd",2));
		end
		
		// Check if we have the write access
		if (section=="allusers") & ~ ATOMSALLUSERSWRITEACCESS then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadAdd",pathconvert(SCI+"/.atoms")));
		end
		
	end
	
	// Define the path of the file that will record the change according to
	// the "section" value
	// =========================================================================
	atoms_directory = atomsPath("system",section);
	
	// Does the atoms_directory exist, if not create it
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		mkdir(atoms_directory);
	end
	
	// Does the SCIHOME/atoms/autoloaded exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"autoloaded") <> [] then
		autoloaded = mgetl(atoms_directory+"autoloaded");
	end
	
	// If packages no specify the installed section, find it
	// =========================================================================
	
	if size(packages(1,:),"*") == 2 then
		
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
		
		for i=1:size(packages(:,1),"*")
			
			if atomsIsInstalled([packages(i,1) packages(i,2)],section) then
				packages(i,3) = section;
			
			elseif section=="user" & atomsIsInstalled([packages(i,1) packages(i,2)],"allusers") then
				packages(i,3) = "allusers";
				
			elseif section=="allusers" & atomsIsInstalled([packages(i,1) packages(i,2)],"user") then
				mprintf(gettext("%s: The following module is installed in the user section, you cannot add it to the ""autoload"" list for all users:\n"),"atomsAutoloadAdd");
				printf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));
				error("");
			
			else
				mprintf(gettext("%s: The following module is not installed:\n"),"atomsAutoloadAdd");
				mprintf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));
			
			end
			
		end
		
	else
		
		for i=1:size(packages(:,1),"*")
			
			// Check if modules are installed
			if ~ atomsIsInstalled([packages(i,1) packages(i,2)],packages(i,3)) then
				mprintf(gettext("%s: The following modules is not installed:\n"),"atomsAutoloadAdd");
				mprintf("\t - ''%s - %s'' (''%s'' section)\n",packages(i,1),packages(i,2),packages(i,3));
				error("");
			end
			
			// A module installed in the user section cannot be add in the 
			// "autoload" list of all users
			
			if (section=="allusers") & (packages(i,3)=="user") then
				mprintf(gettext("%s: The following module is installed in the user section, you cannot add it to the ""autoload"" list for all users:\n"),"atomsAutoloadAdd");
				mprintf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));
				error("");
			end
			
		end
		
	end
	
	// Loop on each module specified as input argument
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		// Add the package only if it doesn't already exist
		
		if find(autoloaded == packages(:,1)+" - "+packages(:,2)+" - "+packages(:,3)) == [] then
			nbAdd      = nbAdd + 1;
			autoloaded = [ autoloaded ; packages(:,1)+" - "+packages(:,2)+" - "+packages(:,3) ];
		end
		
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		mputl(autoloaded,atoms_directory+"autoloaded");
	end
	
endfunction
