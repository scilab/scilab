// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Delete an atoms package from the list of available package

function nbDel = atomsInstallUnregister(name,version,allusers)
	
	rhs                  = argn(2);
	nbDel                = 0;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsInstallUnregister",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",2));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallUnregister",1,2));
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 2 then
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
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstallUnregister",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstallUnregister",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Define the path of the files that will record the change according to
	// the "allusers" value and the existence of the latter
	// =========================================================================
	
	atoms_files = [];
	
	if fileinfo( pathconvert(SCIHOME+"/atoms/installed",%F) )<> [] then
		atoms_files = [ atoms_files ; pathconvert(SCIHOME+"/atoms/installed",%F) ];
	end
	
	if allusers & (fileinfo( pathconvert(SCI+"/.atoms/installed",%F) )<>[]) then
		atoms_files = [ atoms_files ; pathconvert(SCI+"/.atoms/installed",%F) ];
	end
	
	// Loop on each installed file specified as first input argument
	// =========================================================================
	
	for i=1:size(atoms_files,"*")
		
		// Get the installed package list in this file
		installed = mgetl(atoms_files(i));
		
		// Loop on each URL specified as first input argument
		for j=1:size(name,"*")
			indice = find( installed == name(j)+" - "+version(j) );
			if indice <> [] then
				installed(indice) = [];
				nbDel = nbDel + 1;
			end
		end
		
		if installed == [] then
			mdelete(atoms_files(i));
		else
			// Apply changes on this file
			mputl(installed,atoms_files(i));
		end
	end
	
endfunction
