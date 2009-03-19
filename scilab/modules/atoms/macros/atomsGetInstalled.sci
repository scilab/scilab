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

// End user function

// List of installed toolboxes

function tboxes = atomsGetInstalled()
	
	rhs = argn(2)
	
	// Check input parameters
	if rhs > 0 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsGetInstalled",0));
	end
	
	atoms_directories = atomsToolboxDirectory();
	tboxes            = [];
	
	for i=1:size(atoms_directories,"*")
		files = listfiles(atoms_directories(i));
		for j=1:size(files, 1)
			

			tbox = atoms_directories(i) + "/" + files(j)
			if (  ( fileinfo(tbox + "/loader.sce") <> [] ) ..
				& ( fileinfo(tbox + "/DESCRIPTION") <> [] ) ..
				& ( fileinfo(tbox + "/DESCRIPTION-FUNCTIONS") <> [] ) ) then
				tboxes = [tboxes;tbox];
			end
		end
	end
	
	return tboxes;
	
endfunction
