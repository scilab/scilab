// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Return a 5xn string matrix :
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
	
	// Load all packages, "user" section or "allusers" section packages ?
	// =========================================================================
	
	if rhs == 0 then
		allusers = %T;
	else
		// Just check if it's a boolean
		if (type(allusers) <> 4) & (type(allusers) <> 10) then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or a single string expected.\n"),"atomsGetInstalled",1));
		end
	end
	
	if (rhs>=1) & (type(allusers) == 10) & (and(allusers<>["user","allusers","all"])) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsGetInstalled",1));
	end
	
	// Call atomsLoadInstalledMat
	// =========================================================================
	
	// all packages
	// ------------------------------------
	if ((type(allusers)==4) & allusers) | ..
			((type(allusers)==10) & (allusers=="all")) then
		packages = atomsLoadInstalledMat("all");
	
	
	// just "user" section packages
	// ------------------------------------
	elseif ((type(allusers)==4) & ~allusers) | ..
			((type(allusers)==10) & (allusers=="user")) then
		packages = atomsLoadInstalledMat(%F);
	
	
	// just "user" section packages
	// ------------------------------------
	elseif (type(allusers)==10) & (allusers=="allusers") then
		// just "allusers" section packages
		packages = atomsLoadInstalledMat(%T);
	end
	
endfunction
