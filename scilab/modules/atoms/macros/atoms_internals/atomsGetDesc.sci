// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Get the Full Description of a toolbox

function description = atomsGetDesc(name)
	
	// Check input arguments
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsGetDesc",1));
	end
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDesc",2));
	end
	
	DESCRIPTION           = "";
	DESCRIPTIONFUNCTIONS  = "";
	
	// First case, name = absolute/relative path of the DESCRIPTION file
	if regexp(name,"/\/DESCRIPTION$/") <> [] then
		if fileinfo(name) <> [] then
			DESCRIPTION = pathconvert(name,%f);
		else
			error(msprintf(gettext("%s: The description file you''ve enterer as input argument (%s) doesn''t exist.\n"),"atomsGetDesc",name));
		end
	
	// Second Case, name = absolute/relative path of the toolbox 
	elseif isdir(name) then
		check_file = pathconvert(name)+"DESCRIPTION";
		if fileinfo(check_file) <> [] then
			DESCRIPTION = check_file;
		end
		
	// third case, name = Just the name of the toobox, we have to search in the 
	// atoms standard directory
	else
		atoms_directories = atomsToolboxDirectory();
		
		for k=1:size(atoms_directories,"*")
			check_file      = pathconvert(atoms_directories(k)+"/"+name)+"DESCRIPTION";
			if fileinfo(check_file) <> [] then
				DESCRIPTION = check_file;
				break;
			end
		end
		
		// Fourth case, try to download the DESCRIPTION file
		if DESCRIPTION == "" then
			DESCRIPTION = download_file(name,"DESCRIPTION");
		end
		
	end
	
	// No DESCRIPTION file found
	if DESCRIPTION == "" then
		error(msprintf(gettext("%s: No DESCRIPTION file was found.\n"),"atomsGetDesc"));
	end
	
	// Now check if the DESCRIPTION-FUNCTION exists
	DESCRIPTIONFUNCTIONS = DESCRIPTION+"-FUNCTIONS";
	
	disp(DESCRIPTION);
	
	if fileinfo(DESCRIPTIONFUNCTIONS) == [] then
		error(msprintf(gettext("%s: The DESCRIPTION-FUNCTIONS file was found.\n"),"atomsGetDesc"));
	end
	
	// Creation the template of description hash
	[mandatory,optional] = atomsConstant();
	
	for i=1:size(mandatory,"*")
		description(mandatory(i))= []
	end
	
	for i=1:size(optional,"*")
		description(optional(i))= []
	end
	
	// Filling of the hash table with the DESCRIPTION file information
	description = atomsReadDesc(DESCRIPTION,description)
	
	// Filling of the hash table with the DESCRIPTION-FUNCTIONS file information 
	description("Functions") = atomsReadDescFunctions(DESCRIPTIONFUNCTIONS);
	
endfunction

// =============================================================================
// result = download_file(toolboxname,filename)
// Try to download the file (DESCRIPTION OR DESCRIPTION-FUNCTIONS) of the 
// toolbox "toolboxname"
//
// if success : return the absolute path of the downloaded file
// else : return an empty string
//
// Date : 20/03/2009
// =============================================================================

function result = download_file(toolboxname,filename)
	
	mirrors = atomsOfclRepositories();
	result  = "";
	
	if ~isdir(pathconvert(TMPDIR+"/atoms")) then
		mkdir(pathconvert(TMPDIR+"/atoms"));
	end
	
	for i=1:size(mirrors,"*")
		url            = mirrors(i)+"/"+toolboxname+"/"+filename;
		file_out       = TMPDIR+"/atoms/"+filename;
		[rep,stat,err] = unix_g("wget "+url + " -O " + file_out)
		if stat == 0 then
			// Download successfull
			result = file_out;
			break;
		end
	end
	
endfunction
