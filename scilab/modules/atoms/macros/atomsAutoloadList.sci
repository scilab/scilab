// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// get the list of repositories

function modules = atomsAutoloadList(level)
	
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
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadList",0,1));
	end
	
	// Check input argument type (if any)
	// =========================================================================
	
	if (rhs==1) & (type(level) <> 10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
	end
	
	// Check input argument dimension (if any)
	// =========================================================================
	
	if (rhs==1) & (size(level,"*")<>1) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsAutoloadList",1));
	end
	
	// Check input argument values (if any)
	// =========================================================================
	
	if (rhs==1) & (and(level<>["user","allusers"])) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsAutoloadList",1));
	end
	
	// Define the needed paths
	// =========================================================================
	
	allusers_autoload = pathconvert(SCI+"/.atoms/autoloaded",%F);
	user_autoload     = pathconvert(SCIHOME+"/atoms/autoloaded",%F);
	
	// All users autoload
	// =========================================================================
	
	if (rhs == 0) | ((rhs == 1) & (level == "allusers")) then
		if fileinfo(allusers_autoload) <> [] then
			module_list = mgetl(allusers_autoload);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "allusers" ];
			end
		end
	end
	
	// User repositories
	// =========================================================================
	
	if (rhs == 0) | ((rhs == 1) & (level == "user")) then
		if fileinfo(user_autoload) <> [] then
			module_list = mgetl(user_autoload);
			for i=1:size(module_list,"*")
				modules = [ modules ; module_list(i)  "user" ];
			end
		end
	end
	
endfunction
