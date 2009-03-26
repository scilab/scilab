// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Parsing of the DESCRIPTION file

// =============================================================================
// description_out = atomsReadDesc(file_in,description_in)
// Parse a DESCRIPTION file
// 
// Date : 20/03/2009
// =============================================================================

function description_out = atomsReadDesc(file_in,description_in)
	
	description_out = struct();
	
	rhs  = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsReadDesc",1,2));
	end
	
	if regexp( file_in,"/DESCRIPTION$/") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that end with DESCRIPTION expected.\n"),"atomsReadDesc",1));
	end
	
	if rhs==2 & type(description_in)<>17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: mlist expected.\n"),"atomsReadDesc",2));
	end
	
	if rhs==2 then
		description_out = description_in;
	end
	
	tabDesc         = mgetl(file_in);
	current_field   = "";
	
	for i=1:size(tabDesc,"*")
		
		// First case : new field
		if regexp(tabDesc(i),"/^[a-zA-Z0-9]*:\s/","o") == 1 then
			current_field_length    = regexp(tabDesc(i),"/:\s/","o")
			current_field           = part(tabDesc(i),1:current_field_length-1);
			current_value           = part(tabDesc(i),current_field_length+2:length(tabDesc(i)));
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
		error(msprintf(gettext("%s: The description is not well formated at line %d\n"),"atomsReadDesc",i));
		
	end
	
endfunction
