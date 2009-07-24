// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Add a package description to a DESCRIPTION struct
// The DESCRIPTION struct looks like that

// tree_in  =
//  
//    toolbox_1:                     [1x1 struct]
//      2.0:                         [1x1 struct]
//        Toolbox: "toolbox_2"
//        Title: "Toolbox Test 2"
//        Version: "2.0"
//        ...
//      1.0:                         [1x1 struct]
//        Toolbox: "toolbox_2"
//        Title: "Toolbox Test 2"
//        Version: "1.0"
//    ...

function tree_out = atomsDESCRIPTIONadd( tree_in , package_name , package_version , description )
	
	// Check input parameters number
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 4 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONadd",4));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(tree_in) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONadd",1));
	end
	
	if type(package_name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONadd",2));
	end
	
	if type(package_version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONadd",3));
	end
	
	if type(description) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONadd",4));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(package_name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONadd",2));
	end
	
	if size(package_version,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONadd",3));
	end
	
	// And now ... action
	// =========================================================================
	
	tree_out = tree_in;
	
	if isfield(tree_out,package_name) then
		package_name_struct = tree_out(package_name);
	else
		package_name_struct = struct();
	end
	
	package_name_struct(package_version) = description;
	tree_out(package_name)               = package_name_struct;
	
endfunction
