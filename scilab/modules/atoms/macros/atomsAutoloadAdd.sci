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

function nbAdd = atomsAutoloadAdd(name,version,section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs            = argn(2);
	nbAdd          = 0;
	autoloaded     = []; // Column vector that contain autoloaded packages
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadAdd",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadAdd",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadAdd",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsAutoloadAdd",1,2));
	end
	
	// Allusers/user management
	//   - If section is equal to "allusers", module(s) will added to the 
	//     "autoload" list of the "allusers" section :
	//       → SCI/.atoms/autoloaded
	//   - Otherwise, module(s) will added to the "autoload" list of the "user"
	//     section
	//       → SCIHOME/atoms/autoloaded
	// =========================================================================
	
	if rhs <= 2 then
		
		// By default: 
		//  → Add the module to the "autoload" list of the "allusers" section
		//    if we have the write access to SCI directory
		//  → Add the module to the "autoload" list of the "user" section otherwise
		
		if atomsAUWriteAccess() then
			section = "allusers"; 
		else
			section = "user";
		end
		
	else
		
		// Process the 2nd input argument : allusers
		// Allusers can be equal to "user" or "allusers"
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsAutoloadAdd",3));
		end
		
		if and(section<>["user","allusers"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsAutoloadAdd",3));
		end
		
		// Check if we have the write access
		if (section=="allusers") & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadAdd",3,pathconvert(SCI+"/.atoms")));
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
	
	// Check if all input modules are installed
	// =========================================================================
	
	if ~ and(atomsIsInstalled([name version])) then
		mprintf(gettext("%s: The following modules are not installed:\n"),"atomsAutoloadAdd");
		for i=1:size(name,"*")
			if ~ atomsIsInstalled([name(i) version(i)]) then
				mprintf(gettext("\t - ''%s - %s''\n"),name(i),version(i));
			end
		end
		error("");
	end
	
	// A module installed in the user section cannot be add in the "autoload" list 
	// of all users
	
	if (rhs>=3) & (section=="allusers") & (~ atomsIsInstalled([name version],"allusers")) then
		mprintf(gettext("%s: The following modules are installed in the user section, you cannot add them in the ""autoload"" list for all users:\n"),"atomsAutoloadAdd");
		for i=1:size(name,"*")
			if ~ atomsIsInstalled([name(i) version(i)],"allusers") then
				mprintf(gettext("\t - ''%s - %s''\n"),name(i),version(i));
			end
		end
		error("");
	end
	
	// Loop on each module specified as input argument
	// =========================================================================
	
	for i=1:size(name,"*")
		
		// Add the package only if it doesn't already exist
		
		if find(autoloaded == name(i)+" - "+version(i)) == [] then
			nbAdd      = nbAdd + 1;
			autoloaded = [ autoloaded ; name(i)+" - "+version(i) ];
		end
		
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		mputl(autoloaded,atoms_directory+"autoloaded");
	end
	
endfunction
