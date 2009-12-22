// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the full description of 
//  - TOOLBOXES file present in the differents repositories
//  - DESCRIPTION file present in one package

// DESCRIPTION
// |
// |-- packages                              [1x1 struct]
// |   |-- toolbox_1                         [1x1 struct]
// |   |   |-- 2.0                           [1x1 struct] 
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "2.0"
// |   |   |   `-- ..
// |   |   `-- 1.0                           [1x1 struct]
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "1.0"
// |   |   |   `-- ..
// |   |-- module_lycee
// |   `-- ..
// |
// |-- categories                            [1x1 struct]
// |   |-- Optimization                      [1x1 struct]
// |   |   |-- Linear
// |   |   `-- General
// |   `-- ..
// |
// |-- categories_flat                       [1x1 struct]
//     |-- Optimization - Linear
//     |   `-- [ "Optimization" ; "Linear" ]
//     |-- Optimization - General
//     `-- Optimization - General

function description_out = atomsDESCRIPTIONread(file_in)
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONread",1));
	end
	
	if regexp( file_in,"/(TOOLBOXES|DESCRIPTION)/") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that contain ''TOOLBOXES'' or ''DESCRIPTION'' expected.\n"),"atomsDESCRIPTIONread",1));
	end
	
	// Init the output argument
	// =========================================================================
	
	packages         = struct();
	categories_flat  = struct();
	categories       = struct();
	description_out  = struct();
	
	description_out("packages")        = packages;
	description_out("categories")      = categories;
	description_out("categories_flat") = categories_flat;
	
	// Start Read the file
	// =========================================================================
	
	lines_in         = mgetl(file_in);
	current_toolbox  = struct();
	current_field    = "";
	
	if isempty(lines_in) then
		return;
	end
	
	for i=1:(size(lines_in,"*")+1)
		
		// File totally read : register the latest toolbox
		if i == (size(lines_in,"*")+1) then
			
			if and(isfield(current_toolbox,["Toolbox";"Version"])) then
				
				if  ~ isfield(packages,current_toolbox("Toolbox")) then
					// Il s'agit de la première version de la toolbox trouvée
					this_toolbox = struct();
				else
					// On récupère la liste des versions de cette toolbox
					this_toolbox = packages(current_toolbox("Toolbox"));
				end
				
				if isfield(current_toolbox,"ScilabVersion") then
					if atomsIsCompatible(current_toolbox("ScilabVersion")) then
						this_toolbox(current_toolbox("Version")) = current_toolbox;
					end
				else
					error(msprintf(gettext("%s: The file ""%s"" is not well formated, the toolbox ""%s - %s"" doesn''t contain the ScilabVersion field\n"), ..
						"atomsDESCRIPTIONread",..
						file_in,current_toolbox("Toolbox"),..
						current_toolbox("Version")));
				end
				
				packages(current_toolbox("Toolbox")) = this_toolbox;
			end
			
			break;
		end
		
		// First case : new field
		if regexp(lines_in(i),"/^[a-zA-Z0-9]*:\s/","o") == 1 then
			
			// Start new version of toolbox
			if regexp(lines_in(i),"/^Toolbox:\s/","o") == 1 then
				
				if and(isfield(current_toolbox,["Toolbox";"Version"])) then
					
					if  ~ isfield(packages,current_toolbox("Toolbox")) then
						// Il s'agit de la première version de la toolbox trouvée
						this_toolbox = struct();
					else
						// On récupère la liste des versions de cette toolbox
						this_toolbox = packages(current_toolbox("Toolbox"));
					end
					
					if isfield(current_toolbox,"ScilabVersion") then
						if atomsIsCompatible(current_toolbox("ScilabVersion")) then
							this_toolbox(current_toolbox("Version")) = current_toolbox;
						end
					else
						error(msprintf(gettext("%s: The file ""%s"" is not well formated, the toolbox ""%s - %s"" doesn''t contain the ScilabVersion field\n"), ..
							"atomsDESCRIPTIONread",..
							file_in,current_toolbox("Toolbox"),..
							current_toolbox("Version")));
					end
					
					packages(current_toolbox("Toolbox")) = this_toolbox;
				end
				
				// Reset the current_toolbox struct
				current_toolbox  = struct();
			end
			
			// process field
			current_field_length           = regexp(lines_in(i),"/:\s/","o")
			current_field                  = part(lines_in(i),1:current_field_length-1);
			current_value                  = part(lines_in(i),current_field_length+2:length(lines_in(i)));
			current_toolbox(current_field) = current_value;
			
			// Category management
			if (current_field == "Category") & (~ isfield(categories_flat,current_value))  then
				atomsAddCategory(current_value);
			end
			
			continue;
		end
		
		// Second case : Current field continuation
		if regexp(lines_in(i),"/^\s/","o") == 1 then
			current_value = part(lines_in(i),2:length(lines_in(i)));
			current_toolbox(current_field) = [ current_toolbox(current_field) ; current_value ];
			
			// Category management
			if (current_field == "Category") & (~ isfield(categories_flat,current_value)) then
				atomsAddCategory(current_value);
			end
			
			continue;
		end
		
		// Third case : Blank line
		if length(lines_in(i)) == 0 then
			continue;
		end
		
		// Fourth case : Delimiter
		if regexp(lines_in(i),"/^\/\//","o") == 1 then
			continue;
		end
		
		// Else Error
		error(msprintf(gettext("%s: The file ""%s"" is not well formated at line %d\n"),"atomsDESCRIPTIONread",filein,i));
		
	end
	
	description_out("packages")        = packages;
	description_out("categories")      = categories;
	description_out("categories_flat") = categories_flat;
	
endfunction

// =============================================================================
// atomsAddCategory
// =============================================================================

function atomsAddCategory(category_id)
	
	category_main = "";
	category_sub  = "";
	
	pattern_index = regexp(category_id,"/\s-\s/","o");
	
	if pattern_index <> [] then
		category_main                = part(category_id,1:pattern_index-1);
		category_sub                 = part(category_id,pattern_index+3:length(category_id) );
		categories_flat(category_id) = [ category_main ; category_sub ];
	else
		category_main = category_id;
		categories_flat(category_id) = [ category_main ];
	end
	
	if isfield(categories,category_main) then
		if category_sub <> "" then
			subcategories             = categories(category_main);
			subcategories             = [ subcategories ; category_sub ];
			categories(category_main) = subcategories;
		end
	else
		if category_sub == "" then
			categories(category_main) = [];
		else
			categories(category_main) = category_sub;
		end
	end
	
endfunction
