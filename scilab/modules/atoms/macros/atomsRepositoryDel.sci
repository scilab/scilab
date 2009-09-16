// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Remove an URL to the list of repositories, and returns

function nbDel = atomsRepositoryDel(url,allusers)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs   = argn(2);
	nbDel = 0;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositoryDel",1,2));
	end
	
	// Check URLs specified as first input argument
	// =========================================================================
	
	if type(url) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRepositoryDel",1));
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
	
	// Define the path of the files that will record the change according to
	// the "allusers" value and the existence of the latter
	// =========================================================================
	
	atoms_files = [];
	
	if fileinfo( pathconvert(SCIHOME+"/atoms/repositories",%F) )<> [] then
		atoms_files = [ atoms_files ; pathconvert(SCIHOME+"/atoms/repositories",%F) ];
	end
	
	if allusers & (fileinfo( pathconvert(SCI+"/.atoms/repositories",%F) )<>[]) then
		atoms_files = [ atoms_files ; pathconvert(SCI+"/.atoms/repositories",%F) ];
	end
	
	// Loop on each repositories file specified as first input argument
	// =========================================================================
	
	for i=1:size(atoms_files,"*")
		
		// Get the URLs list in this file
		repositories = mgetl(atoms_files(i));
		
		// Loop on each URL specified as first input argument
		for j=1:size(url,"*")
			indice = find( repositories == url(j) );
			if indice <> [] then
				repositories(indice) = [];
				nbDel = nbDel + 1;
			end
		end
		
		if repositories == [] then
			mdelete(atoms_files(i));
		else
			// Apply changes on this file
			mputl(repositories,atoms_files(i));
		
			
		end
	end
	
	// Delete the packages file (created by atomsGetTOOLBOXES) to force reload
	// the different distant TOOLBOXES files
	// =========================================================================
	
	if nbDel > 0 then
		atomsGetTOOLBOXES(%T);
	end
	
endfunction
