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

function nbAdd = atomsAddAutoload(name,version,allusers)
	
	rhs            = argn(2);
	nbAdd          = 0;
	autoloaded     = []; // Column vector that contain autoloaded packages
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAddAutoload",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAddAutoload",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAddAutoload",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsAddAutoload",1,2));
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 2 then
		// By default, The toolbox is autoloaded for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsAddAutoload",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAddAutoload",2,pathconvert(SCI+"/.atoms")));
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
	
	// Does the SCIHOME/atoms/autoloaded exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"autoloaded") <> [] then
		autoloaded = mgetl(atoms_directory+"autoloaded");
	end
	
	// Loop on each URL specified as input argument
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
