// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add toolboxes to the list of packages that are automaticaly loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/config

function nbChanges = atomsSetConfig(field,value)
	
	rhs       = argn(2);
	nbChanges = 0;
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsSetConfig",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(field) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsSetConfig",1));
	end
	
	if type(value) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsSetConfig",2));
	end
	
	// field and value must have the same size
	// =========================================================================
	
	if or( size(field) <> size(value) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsSetConfig",1,2));
	end
	
	// Define the path of the file that will record the change
	// =========================================================================
	atoms_directory =  atomsPath("system","user");
	
	// Does the atoms_directory exist, if not create it
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		mkdir(atoms_directory);
	end
	
	// Get the current config struct
	// =========================================================================
	
	config_struct = atomsGetConfig();
	
	// Loop on field
	// =========================================================================
	
	for i=1:size(field,"*")
		
		if (~isfield(config_struct,field(i))) | (config_struct(field(i))<> value(i)) then
			nbChanges = nbChanges + 1;
		else
			continue;
		end
		
		config_struct(field(i)) = value(i);
	end
	
	// Shortcut
	// =========================================================================
	if nbChanges == 0 then
		return;
	end
	
	// Apply Changes
	// =========================================================================
	
	config_fields      = getfield(1,config_struct);
	config_fields(1:2) = [];
	config_fields      = gsort(config_fields);
		
	config_str = [];
	
	for i=1:size(config_fields,"*")
		config_str = [ config_str ; config_fields(i) + " = " + config_struct(config_fields(i)) ];
	end
	
	mputl(config_str,atoms_directory+"config");
	
endfunction
