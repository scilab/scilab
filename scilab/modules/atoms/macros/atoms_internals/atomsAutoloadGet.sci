// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function packages = atomsAutoloadGet(section)
	
	rhs      = argn(2);
	packages = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsAutoloadGet",1));
	end
	
	// Allusers/user management
	//   - If Allusers is equal to "all", Get both "user" and "allusers" autoload list
	//       → SCI/.atoms/autoloaded
	//       → SCIHOME/atoms/autoloaded
	//   - If Allusers is equal to "allusers",Get only "allusers" autoload list
	//       → SCI/.atoms/autoloaded
	//   - If Allusers is equal to "user",Get only "user" autoload list
	//       → SCIHOME/atoms/autoloaded
	// =========================================================================
	
	if rhs < 1 then
		section = "all";
	
	else
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsAutoloadGet",1));
		end
		
		if size(section,"*")<>1 then
			error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsAutoloadGet",1));
		end
		
		if and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadGet",1));
		end
		
	end
	
	// Define the needed paths
	// =========================================================================
	
	allusers_autoload = atomsPath("system","allusers") + "autoloaded";
	user_autoload     = atomsPath("system","user") + "autoloaded";
	autoloaded_files  = [];
	
	// All users autoload
	// =========================================================================
	
	if or(section==["all";"allusers"]) then
		if fileinfo(allusers_autoload) <> [] then
			autoloaded_files = [ autoloaded_files ; allusers_autoload "allusers" ];
		end
	end
	
	// User repositories
	// =========================================================================
	
	if or(section==["all";"user"]) then
		if fileinfo(user_autoload) <> [] then
			autoloaded_files = [ autoloaded_files ; user_autoload "user" ];
		end
	end
	
	// Loop on each "autoloaded" file
	// =========================================================================
	
	for i=1:size(autoloaded_files(:,1),"*")
		
		// Get the autoloaded package list in this file
		autoloaded = mgetl(autoloaded_files(i,1));
		
		// Loop on each URL specified as first input argument
		for j=1:size(autoloaded,"*")
			current_name_length  = regexp(autoloaded(j),"/\s-\s/","o");
			current_name         = part(autoloaded(j),1:current_name_length-1);
			current_version      = part(autoloaded(j),current_name_length+3:length(autoloaded(j)));
			
			if atomsIsInstalled([current_name current_version],"user") then
				current_path = pathconvert(atomsPath("install","user")+current_name+"/"+current_version,%F);
			else
				current_path = pathconvert(atomsPath("install","allusers")+current_name+"/"+current_version,%F);
			end
			
			packages = [ packages ; current_name current_version autoloaded_files(i,2) current_path];
		end
	end
	
endfunction
