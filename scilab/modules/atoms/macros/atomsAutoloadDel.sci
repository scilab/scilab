// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Remove toolboxes to the list of packages that are automaticaly loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/autoloaded

// End-User function

function nbDel = atomsAutoloadDel(packages,section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check write access on allusers zone
	// =========================================================================
	ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();
	
	rhs   = argn(2);
	nbDel = 0;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadDel",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadDel",1));
	end
	
	if (size(packages(1,:),"*") > 3) | (size(packages(1,:),"*") < 2) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 or mx3 string matrix expected.\n"),"atomsAutoloadDel",1));
	end
	
	// Allusers/user management
	//   - If Allusers is equal to "all", module(s) will removed from both "user"
	//     and "allusers" list
	//       → SCI/.atoms/autoloaded
	//       → SCIHOME/atoms/autoloaded
	//   - If Allusers is equal to "allusers", module(s) will removed from the
	//     "allusers" list
	//       → SCI/.atoms/autoloaded
	//   - If Allusers is equal to "user", module(s) will removed from the
	//     "user" list
	//       → SCIHOME/atoms/autoloaded
	// =========================================================================
	
	if rhs < 2 then
		
		if ATOMSALLUSERSWRITEACCESS then
			section = "all"; 
		else
			section = "user";
		end
	
	else
		// Process the 2nd input argument : allusers
		// Allusers can equal to "user","allusers" or "all"
		
		if (type(section) <> 4) & (type(section) <> 10) then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or a single string expected.\n"),"atomsAutoloadDel",2));
		end
		
		if (type(section) == 10) & and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadDel",2));
		end
		
		// Check if we have the write access
		if or(section==["all","allusers"]) & ~ ATOMSALLUSERSWRITEACCESS then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadDel",pathconvert(SCI+"/.atoms")));
		end
	end
	
	// If packages no specify the installed section
	// =========================================================================
	
	if size(packages(1,:),"*") == 2 then
		packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
	end
	
	// Define the path of the files that will record the change according to
	// the "allusers" value and the existence of the latter
	// =========================================================================
	atoms_files = atomsPath("system",section) + "autoloaded";
	
	// Loop on each installed file specified as first input argument
	// =========================================================================
	
	for i=1:size(atoms_files,"*")
		
		if fileinfo(atoms_files(i))==[] then
			continue;
		end
		
		// Get the installed package list in this file
		autoloaded = mgetl(atoms_files(i));
		
		// Loop on each package specified as first input argument
		for j=1:size(packages(:,1),"*")
			
			if isempty(packages(j,3)) | (packages(j,3)=="all") then
				search_str = packages(j,1)+" - "+packages(j,2)+" - "+["allusers";"user"];
			else
				search_str = packages(j,1)+" - "+packages(j,2)+" - "+packages(j,3);
			end
			
			for k=1:size(search_str,"*")
				if find(autoloaded == search_str(k)) <> [] then
					nbDel = nbDel + 1;
					autoloaded(find(autoloaded == search_str(k))) = [];
				end
			end
		end
		
		if autoloaded == [] then
			mdelete(atoms_files(i));
		else
			// Apply changes on this file
			mputl(autoloaded,atoms_files(i));
		end
		
	end
	
endfunction
