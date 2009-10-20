// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// get the list of repositories

function modules = atomsAutoloadList(section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs      = argn(2);
	modules  = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsAutoloadList",1));
	end
	
	// Allusers/user management
	//   - If Allusers is equal to "all", display both "user" and "allusers" autoload list
	//       → SCI/.atoms/autoloaded
	//       → SCIHOME/atoms/autoloaded
	//   - If Allusers is equal to "allusers",display only "allusers" autoload list
	//       → SCI/.atoms/autoloaded
	//   - If Allusers is equal to "user",display only "user" autoload list
	//       → SCIHOME/atoms/autoloaded
	// =========================================================================
	
	if rhs < 1 then
		section = "all";
	
	else
		
		if type(section) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
		end
		
		if size(section,"*")<>1 then
			error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
		end
		
		if and(section<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadList",1));
		end
		
	end
	
	// All users autoload
	// =========================================================================
	
	if or(section==["all";"allusers"]) then
		autoloaded_file = atomsPath("system","allusers") + "autoloaded";
		if fileinfo(autoloaded_file) <> [] then
			module_list = mgetl(autoloaded_file);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "allusers" ];
			end
		end
	end
	
	// User repositories
	// =========================================================================
	
	if or(section==["all";"user"]) then
		autoloaded_file = atomsPath("system","user") + "autoloaded";
		if fileinfo(autoloaded_file) <> [] then
			module_list = mgetl(autoloaded_file);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "user" ];
			end
		end
	end
	
endfunction
