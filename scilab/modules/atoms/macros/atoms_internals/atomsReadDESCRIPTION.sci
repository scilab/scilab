// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the full description of the TOOLBOXES present in the differents repositories

function description_out = atomsReadDESCRIPTION(file_in,description_in)
	
	description_out = struct();
	
	rhs  = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsReadDESCRIPTION",1,2));
	end
	
	if regexp( file_in,"/TOOLBOXES/") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that contain TOOLBOXES expected.\n"),"atomsReadDESCRIPTION",1));
	end
	
	if rhs==2 & type(description_in)<>17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: mlist expected.\n"),"atomsReadDESCRIPTION",2));
	end
	
	if rhs==2 then
		description_out = description_in;
	end
	
	TOOLBOXES        = mgetl(file_in);
	
	current_toolbox  = struct();
	current_field    = "";
	
	for i=1:size(TOOLBOXES,"*")
		
		// First case : new field
		if regexp(TOOLBOXES(i),"/^[a-zA-Z0-9]*:\s/","o") == 1 then
			
			// Start new version of toolbox
			if regexp(TOOLBOXES(i),"/^Toolbox:\s/","o") == 1 then
				
				if and(isfield(current_toolbox,["Toolbox";"Version"])) then
					
					if  ~ isfield(description_out,current_toolbox("Toolbox")) then
						// Il s'agit de la première version de la toolbox trouvée
						this_toolbox = struct();
					else
						// On récupère la liste des versions de cette toolbox
						this_toolbox = description_out(current_toolbox("Toolbox"));
					end
					
					this_toolbox(current_toolbox("Version")) = current_toolbox;
					description_out(current_toolbox("Toolbox")) = this_toolbox;
				end
				
				// Reset the current_toolbox struct
				current_toolbox  = struct();
			end
			
			// process field
			current_field_length           = regexp(TOOLBOXES(i),"/:\s/","o")
			current_field                  = part(TOOLBOXES(i),1:current_field_length-1);
			current_value                  = part(TOOLBOXES(i),current_field_length+2:length(TOOLBOXES(i)));
			current_toolbox(current_field) = current_value;
			continue;
		end
		
		// Second case : Current field continuation
		if regexp(TOOLBOXES(i),"/^\s/","o") == 1 then
			current_value = part(TOOLBOXES(i),2:length(TOOLBOXES(i)));
			current_toolbox(current_field) = [ current_toolbox(current_field) ; current_value ];
			continue;
		end
		
		// Last line of the file : register the latest toolbox
		if i == size(TOOLBOXES,"*") then
			this_toolbox(current_toolbox("Version")) = current_toolbox;
			description_out(current_toolbox("Toolbox")) = this_toolbox;
		end
		
		// Third case : Blank line
		if length(TOOLBOXES(i)) == 0 then
			continue;
		end
		
		// Fourth case : Delimiter
		if regexp(TOOLBOXES(i),"/^\/\//","o") == 1 then
			continue;
		end
				
		// Else Error
		error(msprintf(gettext("%s: The TOOLBOXES file ("+file_in+") is not well formated at line %d\n"),"atomsReadDESCRIPTION",i));
		
	end
	
endfunction
