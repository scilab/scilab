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

function packages = atomsGetInstalled(allusers)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	rhs      = argn(2);
	packages = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetInstalled",1));
	end
	
	// Load all packages, or just user packages ?
	// =========================================================================
	
	if rhs == 0 then
		allusers = %T;
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetInstalled",1));
		end
	end
	
	// Call atomsLoadInstalledMat
	// =========================================================================
	
	if allusers then
		// all packages
		packages = atomsLoadInstalledMat("all");
	else
		// user packages
		packages = atomsLoadInstalledMat(%F);
	end
	
endfunction
