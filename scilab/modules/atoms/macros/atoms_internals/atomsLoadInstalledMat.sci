// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function matrix_out = atomsLoadInstalledMat(allusers)
	
	rhs = argn(2);
	
	// Init the output argument
	// =========================================================================
	matrix_out = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	// Check number of input argument type
	// =========================================================================
	
	if (type(allusers)<>4) & (type(allusers)<>10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or a string expected.\n"),"atomsLoadInstalledMat",1));
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	
	if (type(allusers)==10) & (allusers=="all") then
		installed_file = [ pathconvert(SCI+"/.atoms/installed.bin",%F); pathconvert(SCIHOME+"/atoms/installed.bin",%F) ];
	elseif allusers then
		installed_file = pathconvert(SCI+"/.atoms/installed.bin",%F);
	else
		installed_file = pathconvert(SCIHOME+"/atoms/installed.bin",%F);
	end
	
	// Loop on installed files
	// =========================================================================
	
	for i=1:size(installed_file,"*")
		
		// Does the file exist,
		// If yes, load the tree
		// If not, the job is done for this file
		// ---------------------------------------------------------------------
		
		if fileinfo(installed_file(i)) <> [] then
			load(installed_file(i),"installed_mat");
		else
			continue
		end
		
		matrix_out = [ matrix_out ; installed_mat ];
	end
	
endfunction
