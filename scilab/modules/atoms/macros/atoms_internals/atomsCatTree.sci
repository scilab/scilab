// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// internal function

// =============================================================================
// tree_out = atomsCatTree( tree_in_1 , tree_in_2 )
//
// Date : 19/05/2009
// =============================================================================

function tree_out = atomsCatTree( tree_in_1 , tree_in_2 )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsCatTree",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree_in_1) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCatTree",1));
	end
	
	if type(tree_in_2) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCatTree",2));
	end
	
	// tree_in_1 is the master tree :
	// =========================================================================
	
	tree_out  = tree_in_1;
	
	// Loop on tree_in_2 fields
	// =========================================================================
	
	fields = getfield(1,tree_in_2);
	fields(1:2) = [];
	
	for i=1:size(fields,"*")
		if ~ isfield(tree_out,fields(i)) then
			tree_out(fields(i)) = tree_in_2(fields(i));
		end
	end
	
endfunction
