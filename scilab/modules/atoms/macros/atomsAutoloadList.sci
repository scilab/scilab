// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// get the list of repositories

function modules = atomsAutoloadList(allusers)
	
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
		allusers = "all";
	
	else
		
		if type(allusers) <> 10 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
		end
		
		if size(allusers,"*")<>1 then
			error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
		end
		
		if and(allusers<>["user","allusers","all"]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadList",1));
		end
		
	end
	
	// Define the needed paths
	// =========================================================================
	
	allusers_autoload = pathconvert(SCI+"/.atoms/autoloaded",%F);
	user_autoload     = pathconvert(SCIHOME+"/atoms/autoloaded",%F);
	
	// All users autoload
	// =========================================================================
	
	if or(allusers==["all";"allusers"]) then
		if fileinfo(allusers_autoload) <> [] then
			module_list = mgetl(allusers_autoload);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "allusers" ];
			end
		end
	end
	
	// User repositories
	// =========================================================================
	
	if or(allusers==["all";"user"]) then
		if fileinfo(user_autoload) <> [] then
			module_list = mgetl(user_autoload);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "user" ];
			end
		end
	end
	
endfunction
