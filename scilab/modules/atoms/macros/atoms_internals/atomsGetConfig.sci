// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Get the configuration of the atoms system

function result = atomsGetConfig(field)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetConfig",0,1));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if  (rhs>0) & (type(field) <> 10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsGetConfig",1));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if (rhs>0) & (size(field,"*") <> 1) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsGetConfig",1));
	end
	
	// Define the default value of the result according to the number of input
	// arguments
	// =========================================================================
	
	if rhs==0 then
		result = struct();
	else
		result = "";
	end
	
	// Define the path of the file that will record the change
	// =========================================================================
	atoms_directory = atomsPath("system","user");
	
	// Does the SCIHOME/atoms/config exist, if yes load it
	// =========================================================================
	
	if fileinfo(atoms_directory+"config") <> [] then
		config_lines = mgetl(atoms_directory+"config");
	else
		return;
	end
	
	// Loop on each URL specified as input argument
	// =========================================================================
	
	for i=1:size(config_lines,"*")
		
		if isempty( stripblanks( config_lines(i) ) ) then
			continue;
		end
		
		if regexp(config_lines(i),"/^[a-zA-Z0-9]*\s=\s/","o") == 1 then
			
			// process field
			current_field_length  = regexp(config_lines(i),"/\s=\s/","o")
			current_field         = part(config_lines(i),1:current_field_length-1);
			current_value         = part(config_lines(i),current_field_length+3:length(config_lines(i)));
			
			if (rhs==1) & (current_field==field) then
				result = current_value;
				return;
			elseif( rhs==0 )
				result(current_field) = current_value;
			end
			
		else
			error(msprintf(gettext("%s: The config file ("+atoms_directory+"config) is not well formated at line %d\n"),"atomsGetConfig",i));
		end
		
	end
	
endfunction
