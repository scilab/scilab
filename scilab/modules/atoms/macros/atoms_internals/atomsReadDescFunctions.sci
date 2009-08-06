// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Parsing of the Description-Function file

function descriptionFunctions = atomsReadDescFunctions(name)
	
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsReadDescFunctions",1));
	end
	
	DESCRIPTIONFUNCTIONS  = "";
	
	// First case, name = absolute/relative path of the DESCRIPTION-FUNCTIONS file
	if regexp(name,"/\/DESCRIPTION-FUNCTIONS$/") <> [] then
		if fileinfo(name) <> [] then
			DESCRIPTIONFUNCTIONS = pathconvert(name,%f);
		else
			error(msprintf(gettext("%s: The description file you''ve enterer as input argument (%s) doesn''t exist.\n"),"atomsReadDescFunctions",name));
		end
	
	// Second Case, name = absolute/relative path of the toolbox 
	elseif isdir(name) then
		check_file = pathconvert(name)+"DESCRIPTION-FUNCTIONS";
		if fileinfo(check_file) <> [] then
			DESCRIPTIONFUNCTIONS = check_file;
		end
		
	// third case, name = Just the name of the toobox, we have to search in the 
	// atoms standard directory
	else
		atoms_directories = atomsToolboxDirectory();
		
		for k=1:size(atoms_directories,"*")
			check_file      = pathconvert(atoms_directories(k)+"/"+name)+"DESCRIPTION-FUNCTIONS";
			if fileinfo(check_file) <> [] then
				DESCRIPTIONFUNCTIONS = check_file;
				break;
			end
		end
		
	end
	
	descriptionFunctions = dfTohash(DESCRIPTIONFUNCTIONS);
	
endfunction

// =============================================================================
// description_out = dfTohash(file_in,description_in)
// Easy filling of the hash table (DESCRIPTION-FUNCTIONS file)
// 
// Date : 20/03/2009
// =============================================================================

function description_out = dfTohash(file_in)
	
	tabDesc         = mgetl(file_in);
	current_field   = "";
	
	for i=1:size(tabDesc,"*")
		
		// First case : new field
		if regexp(tabDesc(i),"/^[a-zA-Z0-9_]*\s-\s/","o") == 1 then
			current_field_length    = regexp(tabDesc(i),"/\s-\s/","o")
			current_field           = part(tabDesc(i),1:current_field_length-1);
			current_value           = part(tabDesc(i),current_field_length+3:length(tabDesc(i)));
			description_out(current_field) = current_value;
			continue;
		end
		
		// Second case : Current field continuation
		if regexp(tabDesc(i),"/^\s/","o") == 1 then
			current_value = part(tabDesc(i),2:length(tabDesc(i)));
			description_out(current_field) = [ description_out(current_field) ; current_value ];
			continue;
		end
		
		// Third case : Blank line
		if length(tabDesc(i)) == 0 then
			continue;
		end
		
		// Else Error
		error(msprintf(gettext("%s: The DESCRIPTION-FUNCTIONS file is not well formated at line %d\n"),"atomsReadDescFunctions",i));
		
	end
	
endfunction
