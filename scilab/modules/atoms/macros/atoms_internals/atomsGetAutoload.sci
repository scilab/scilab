// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

function packages = atomsGetAutoload(allusers)
	
	rhs      = argn(2);
	packages = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetAutoload",1));
	end
	
	// Apply changes for all users or just for me ?
	// =========================================================================
	
	if rhs == 0 then
		allusers = %T;
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetAutoload",1));
		end
	end
	
	// Define the differents path of the file where are autoloaded
	// =========================================================================
	
	autoloaded_files = [];
	user_file        = pathconvert(SCIHOME+"/atoms/autoloaded",%F);
	alluser_file     = pathconvert(SCI+"/.atoms/autoloaded"   ,%F);
	
	if fileinfo(user_file)<>[] then
		autoloaded_files = [ autoloaded_files ; user_file "user" ];
	end
	
	if allusers & (fileinfo(alluser_file)<>[]) then
		autoloaded_files = [ autoloaded_files ; alluser_file "allusers"];
	end
	
	// Loop on each "autoloaded" file specified as first input argument
	// =========================================================================
	
	for i=1:size(autoloaded_files(:,1),"*")
		
		// Get the autoloaded package list in this file
		autoloaded = mgetl(autoloaded_files(i,1));
		
		// Loop on each URL specified as first input argument
		for j=1:size(autoloaded,"*")
			current_name_length  = regexp(autoloaded(j),"/\s-\s/","o");
			current_name         = part(autoloaded(j),1:current_name_length-1);
			current_version      = part(autoloaded(j),current_name_length+3:length(autoloaded(j)));
			
			if autoloaded_files(i,2) == "user" then
				// user
				current_path  = pathconvert(SCIHOME+"/atoms/"+current_name+"/"+current_version,%F); 
			else
				// all users
				current_path = pathconvert(SCI+"/contrib/"+current_name+"/"+current_version,%F);
			end
			
			packages = [ packages ; current_name current_version autoloaded_files(i,2) current_path];
		end
	end
	
endfunction
