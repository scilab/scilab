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

function atomsInstallUnregister(name,version,allusers)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsInstallUnregister",3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",2));
	end
	
	if type(allusers) <> 4 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstallUnregister",3));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallUnregister",1,2));
	end
	
	// Process installed
	// =========================================================================
	installed_before = atomsLoadInstalledStruct(allusers);
	installed_after  = atomsRmfields(installed_before,name+" - "+version);
	atomsSaveInstalled(installed_after,allusers);
	
	// Process installed dependencies
	// =========================================================================
	installed_deps_before = atomsLoadInstalleddeps(allusers);
	installed_deps_after  = atomsRmfields(installed_deps_before,name+" - "+version);
	atomsSaveInstalleddeps(installed_deps_after,allusers);
	
endfunction


function struct_out = atomsRmfields(struct_in,fields_to_remove)
	
	rhs        = argn(2);
	struct_out = struct();
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsRmfields",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(struct_in) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsRmfields",1));
	end
	
	if type(fields_to_remove) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRmfields",1));
	end
	
	// Get the fields of the 1st input argument
	// =========================================================================	
	
	fields_in      = getfield(1,struct_in);
	fields_in(1:2) = [];
	
	for i=1:size( fields_in,"*")
		if find(fields_in(i) == fields_to_remove) == [] then
			struct_out( fields_in(i) ) = struct_in(fields_in(i));
		end
	end
	
endfunction
