// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function struct_out = atomsRmfields(struct_in,fields_to_remove)
	
	rhs        = argn(2);
	struct_out = struct();
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsRmfields",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(struct_in) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsRmfields",1));
	end
	
	if type(fields_to_remove) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRmfields",1));
	end
	
	// Get the fields of the 1st input argument
	// =========================================================================	
	
	fields_in      = getfield(1,struct_in);
	fields_in(1:2) = [];
	
	for i=1:size( fields_in,"*")
		if find(fields_in(i) == fields_to_remove) == [] then
			struct_out( fields_in(i) ) = struct_in(fields_in(i));
		end
	end
	
endfunction
