// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// internal function

function tree_out = atomsDESCRIPTIONcat( tree_in_1 , tree_in_2 )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONcat",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree_in_1) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDESCRIPTIONcat",1));
	end
	
	if type(tree_in_2) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDESCRIPTIONcat",2));
	end
	
	// tree_in_1 is the master tree :
	// =========================================================================
	
	tree_out  = tree_in_1;
	
	// Loop on tree_in_2 fields
	// =========================================================================
	
	package_names      = getfield(1,tree_in_2);
	package_names(1:2) = [];
	
	for i=1:size(package_names,"*")
		
		if isfield(tree_out,package_names(i)) then
			
			// The package already exists in tree_in_1
			package_versions_struct = tree_in_2(package_names(i));
			
			if type(package_versions_struct) <> 17 then
				error(msprintf(gettext("%s: Wrong value for input argument #%d: The matrix oriented typed list is not well formatted.\n"),"atomsDESCRIPTIONcat",1));
			end
			
			package_versions      = getfield(1,package_versions_struct);
			package_versions(1:2) = [];
			package_versions_out  = tree_out(package_names(i));
			
			for j=1:size(package_versions,"*")
				package_versions_out(package_versions(j)) = package_versions_struct(package_versions(j));
			end
			
			tree_out(package_names(i)) = package_versions_out;
			
		else
			// The package doesn't exist in tree_in_1
			tree_out(package_names(i)) = tree_in_2(package_names(i));
		end
		
	end
	
endfunction
