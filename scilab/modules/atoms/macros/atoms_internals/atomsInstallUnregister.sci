// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Delete an atoms package from the list of available package
// This function has an impact on the following files :
//  -> ATOMSDIR/installed
//  -> ATOMSDIR/installed_deps

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
	
	// installed files
	
	atoms_files = [];
	
	if fileinfo( pathconvert(SCIHOME+"/atoms/installed",%F) )<> [] then
		atoms_files = [ atoms_files ; pathconvert(SCIHOME+"/atoms/installed",%F) ];
	end
	
	if allusers & (fileinfo( pathconvert(SCI+"/.atoms/installed",%F) )<>[]) then
		atoms_files = [ atoms_files ; pathconvert(SCI+"/.atoms/installed",%F) ];
	end
	
	// installed_deps files
	
	atoms_files_deps = [];
	
	if fileinfo( pathconvert(SCIHOME+"/atoms/installed_deps",%F) )<> [] then
		atoms_files_deps = [ atoms_files_deps ; pathconvert(SCIHOME+"/atoms/installed_deps",%F) ];
	end
	
	if allusers & (fileinfo( pathconvert(SCI+"/.atoms/installed_deps",%F) )<>[]) then
		atoms_files_deps = [ atoms_files_deps ; pathconvert(SCI+"/.atoms/installed_deps",%F) ];
	end
	
	// Loop on each installed file specified as first input argument
	// =========================================================================
	
	for i=1:size(atoms_files,"*")
		
		// Get the installed package list in this file
		installed = mgetl(atoms_files(i));
		
		// Loop on each URL specified as first input argument
		for j=1:size(name,"*")
			indice = grep(installed,"/^[AI]\s-\s"+name(j)+"\s-\s"+version(j)+"$/","r");
			
			if indice <> [] then
				nbDel = nbDel + 1;
				installed(indice) = [];
			end
		end
		
		if installed == [] then
			mdelete(atoms_files(i));
		else
			// Apply changes on this file
			mputl(installed,atoms_files(i));
		end
	end
	
	// Loop on each installed file specified as first input argument
	// =========================================================================
	
	for i=1:size(atoms_files_deps,"*")
		
		found = 0;
		
		// Get the installed package list in this file
		installed_deps_in  = mgetl(atoms_files_deps(i));
		installed_deps_out = [];
		
		// Loop on each URL specified as first input argument
		for j=1:size(name,"*")
			
			// Loop on each lines of the installed_deps file
			for k=1:size(installed_deps_in,"*")
				
				if installed_deps_in(k) == "["+name(j)+" - "+version(j)+"]" then
					found = 1;
				end
				
				if found == 1 then
					continue;
				end
				
				if regexp(installed_deps_in(k),"/^\[(.)*\]$/","o") <> [] then
					found = 0;
				end
				
				if found == 0 then
					installed_deps_out = [ installed_deps_out , installed_deps_in(k) ];
				end
			end
			
		end
		
		if installed_deps_out == [] then
			mdelete(atoms_files_deps(i));
		else
			// Apply changes on this file
			mputl(installed_deps_out,atoms_files_deps(i));
		end
	end
	
endfunction
