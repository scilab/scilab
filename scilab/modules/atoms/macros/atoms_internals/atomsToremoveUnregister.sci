// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add toolboxes to the list of packages to remove
// This function has an impact on the following files :
//  -> ATOMSDIR/toremove.bin

function atomsToremoveUnregister(name,version,allusers)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsToremoveUnregister",3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveUnregister",1));
	end
	
	if type(version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveUnregister",2));
	end
	
	if type(allusers) <> 4 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsToremoveUnregister",3));
	end
	
	// name and version must have the same size
	// =========================================================================
	
	if or( size(name) <> size(version) ) then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsToremoveUnregister",1,2));
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	
	if allusers then
		atoms_directory = pathconvert(SCI+"/.atoms");
	else
		atoms_directory = pathconvert(SCIHOME+"/atoms");
	end
	
	// Does the atoms_directory exist, Nothing to unregister : quit the function
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		return;
	end
	
	// Define the path of the file that will record the change 
	// =========================================================================
	
	toremove_bin = atoms_directory+"toremove.bin";
	
	// Get the toremove matrix
	// =========================================================================
	
	if fileinfo(toremove_bin) <> [] then
		load(toremove_bin,"toremove_mat");
	else
		return;
	end
	
	// Loop on input arguments
	// =========================================================================
	for i=1:size(name,"*")
		toremove_mat(toremove_mat == [name(i) version(i)]) = [];
	end
	
	// Apply changes
	// =========================================================================
	if isempty(toremove_mat) then
		save(toremove_bin,toremove_mat);
	else
		mdelete(toremove_bin);
	end
	
endfunction
