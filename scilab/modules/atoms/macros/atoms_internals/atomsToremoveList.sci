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

function toremove = atomsToremoveList(allusers)
	
	rhs        = argn(2);
	toremove   = [];
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsToremoveList",1));
	end
	
	// Get only the "user" list or both "user" and "allusers" list
	// =========================================================================
	
	if rhs < 1 then
		// By default, install for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsToremoveList",1));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsToremoveList",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	atoms_directories =  pathconvert(SCIHOME+"/atoms");
	
	if allusers then
		atoms_directories = [ atoms_directories ; pathconvert(SCI+"/.atoms") ];
	end
	
	// Get the toremove matrix
	// =========================================================================
	
	for i=1:size(atoms_directories,"*")
		toremove_bin = atoms_directories(i)+"toremove.bin";
		if fileinfo(toremove_bin) <> [] then
			load(toremove_bin,"toremove_mat");
			toremove = [ toremove ; toremove_mat ];
		end
	end
	
endfunction
