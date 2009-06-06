// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Renvoie la liste des packages dont celui passé en argument est une dépendance

function packages = atomsGetDepParents(name,version,allusers)
	
	rhs      = argn(2);
	packages = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetDepChilds",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepChilds",1));
	end
	
	if type(version)<>10  then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepChilds",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsGetDepChilds",1));
	end
	
	if size(version,"*")<>1  then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsGetDepChilds",2));
	end
	
	// All user management
	// =========================================================================
	
	if rhs == 2 then
		allusers = %T;
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetDepChilds",3));
		end
	end
	
	// Define the differents path of the file where are installed
	// =========================================================================
	
	installed_deps_files = [];
	user_file            = pathconvert(SCIHOME+"/atoms/installed_deps",%F);
	alluser_file         = pathconvert(SCI+"/.atoms/installed_deps",%F);
	
	if fileinfo(user_file)<>[] then
		installed_deps_files = [ installed_deps_files ; user_file "user" ];
	end
	
	if allusers & (fileinfo(alluser_file)<>[]) then
		installed_deps_files = [ installed_deps_files ; alluser_file "alluser"];
	end
	
	// Loop on each "installed_deps" file specified as first input argument
	// =========================================================================
	
	for i=1:size(installed_deps_files(:,1),"*")
		
		// Get the installed package list in this file
		installed_deps_lines = mgetl(installed_deps_files(i,1));
		
		// Loop on each lines
		for j=1:size(installed_deps_lines,"*")
			
			if regexp(installed_deps_lines(j),"/^\[(.)*\]$/","o") <> [] then
				
				// Remove leading and trailing whitespaces.
				this_line = stripblanks(installed_deps_lines(j));
				
				// Remove leading and trailing [ ]
				this_line = part(this_line,2:length(this_line)-1);
				
				current_name_length = regexp(this_line,"/\s-\s/","o");
				current_name        = part(this_line,1:current_name_length-1);
				current_version     = part(this_line,current_name_length+3:length(this_line));
				current_parent      = [current_name current_version];
				
				continue;
			end
			
			if stripblanks(installed_deps_lines(j)) == name+" - "+version then
				if ~ isempty(current_parent) then
					packages = [ packages ; current_parent ];
				end
			end
		end
		
	end
	
endfunction
