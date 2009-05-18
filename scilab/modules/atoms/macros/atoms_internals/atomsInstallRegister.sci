// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function nbAdd = atomsInstallRegister(name,version,allusers)
	
	rhs           = argn(2);
	nbAdd         = 0;
	installed     = []; // Column vector that contain user repositories
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsInstallRegister",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallRegister",1,2));
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 2 then
		// By default, The toolbox is installed for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstallRegister",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstallRegister",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	
	if allusers then
		atoms_directory = pathconvert(SCI+"/.atoms");
	else
		atoms_directory = pathconvert(SCIHOME+"/atoms");
	end
	
	// Does the atoms_directory exist, if not create it
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		mkdir(atoms_directory);
	end
	
	// Does the SCIHOME/atoms/installed exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"installed") <> [] then
		installed = mgetl(atoms_directory+"installed");
	end
	
	// Loop on each URL specified as input argument
	// =========================================================================
	
	for i=1:size(name,"*")
		// Add the URL only if it doesn't already exist
		if find( installed == name(i)+" - "+version(i) ) == [] then
			installed = [ installed ; name(i)+" - "+version(i) ];
			nbAdd = nbAdd + 1;
		end
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		mputl(installed, atoms_directory+"installed");
	end
	
endfunction
