// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Return a 4xn string matrix :
// 
// !toolbox2          0.1  user     /home/pmarecha/.Scilab/scilab-branch-atoms/atoms/contrib/toolbox2/0.1  A  !
// !                                                                                                          !
// !toolbox2          0.2  user     /home/pmarecha/.Scilab/scilab-branch-atoms/atoms/contrib/toolbox2/0.2  A  !
// !                                                                                                          !
// !toolbox_skeleton  1.3  alluser  /home/pmarecha/work/atoms/scilab/contrib/toolbox_skeleton/1.3          I  !

function packages = atomsGetLoaded()
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	packages = [];
	
	// Define the differents path of the file where are installed
	// =========================================================================
	loaded_file   = pathconvert(TMPDIR+"/atoms/loaded",%F);
	
	if fileinfo(loaded_file) == [] then
		packages = [];
		return;
	end
	
	// load the loaded_file
	// =========================================================================
	loaded = mgetl(loaded_file);
	
	// Loop on each URL specified as first input argument
	// =========================================================================
	
	for i=1:size(loaded,"*")
		current_name_length  = regexp(loaded(i),"/\s-\s/","o");
		current_name         = part(loaded(i),1:current_name_length-1);
		current_version      = part(loaded(i),current_name_length+3:length(loaded(i)));
		
		packages = [ packages ; atomsGetInstalledDetails([current_name current_version]) ];
	end
	
endfunction
