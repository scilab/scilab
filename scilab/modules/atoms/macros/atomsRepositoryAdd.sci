// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function nbAdd = atomsRepositoryAdd(url,allusers)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs                    = argn(2);
	nbAdd                = 0;
	repositories         = []; // Column vector that contain user repositories
	currentRepositories  = atomsRepositoryList();
	currentRepositories  = currentRepositories(:,1);
	valid_url_pattern    = "/^((((H|h)(T|t)|(F|f))(T|t)(P|p)((S|s)?))\:\/\/)?(www.|[a-zA-Z0-9].)[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\:[0-9]{1,5})*(\/($|[a-zA-Z0-9\.\,\;\?\''\\\+&amp;%\$#\=~_\-]+))*$/";
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositoryAdd",1,2));
	end
	
	// Check URLs specified as first input argument
	// =========================================================================
	
	if type(url) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRepositoryAdd",1));
	end
	
	for i=1:size(url,"*")
		if ~ regexp(url(i),valid_url_pattern,"o") == 1 then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: This ("+url(i)+") is not a valid URL.\n"),"atomsRepositoryAdd",1));
		end
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 1 then
		// By default, add the repository for all users (if we have write access
		// of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsRepositoryDel",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRepositoryDel",2,pathconvert(SCI+"/.atoms")));
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
	
	// Does the SCIHOME/atoms/repositories exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"repositories") <> [] then
		repositories = mgetl(atoms_directory+"repositories");
	end
	
	// Loop on each URL specified as input argument
	// =========================================================================
	
	for i=1:size(url,"*")
		// Add the URL only if it doesn't already exist
		if find( currentRepositories == url(i) ) == [] then
			repositories = [ repositories ; url(i) ];
			nbAdd = nbAdd + 1;
		end
	end
	
	// Apply changes
	// =========================================================================
	
	if nbAdd > 0 then
		
		mputl(repositories, atoms_directory+"repositories");
		
		// Force reload the different distant TOOLBOXES files
		atomsGetTOOLBOXES(%T);
		
	end
	
endfunction
