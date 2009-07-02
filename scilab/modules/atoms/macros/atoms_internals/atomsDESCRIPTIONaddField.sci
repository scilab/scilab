// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Add a field to the description of one ore more package
// The description struct looks like that

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

function tree_out = atomsDESCRIPTIONaddField( tree_in , package_name , package_version , field , value )
	
	// Check input parameters number
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 5 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONaddField",5));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(tree_in) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONaddField",1));
	end
	
	if type(package_name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",2));
	end
	
	if type(package_version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",3));
	end
	
	if type(field) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",4));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(package_name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",2));
	end
	
	if size(package_version,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",3));
	end
	
	if size(field,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",4));
	end
	
	if size(value,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONaddField",5));
	end
	
	// And now ... action
	// =========================================================================
	
	tree_out = tree_in;
	
	// 1st case : All packages are concerned
	// -------------------------------------------------------------------------
	
	if (package_name == "*") & (package_version == "*") then
		
		package_names      = getfield(1,tree_in);
		package_names(1:2) = [];
		
		for i=1:size(package_names,"*")
			
			package_versions_struct = tree_out(package_names(i));
			package_versions        = getfield(1,package_versions_struct);
			package_versions(1:2)   = [];
			
			for j=1:size(package_versions,"*")
				this_package_struct                          = package_versions_struct(package_versions(j));
				this_package_struct(field)                   = value;
				package_versions_struct(package_versions(j)) = this_package_struct;
			end
			
			tree_out(package_names(i)) = package_versions_struct;
		end
	
	
	// 2nd case : All versions of a package are concerned
	// -------------------------------------------------------------------------
	
	elseif  package_version == "*" then
		
		if ~ isfield(tree_out,package_name) then
			error(msprintf(gettext("%s: The package ''%s'' is not present in the struct.\n"),"atomsDESCRIPTIONaddField",package_name));
		end
		
		package_versions_struct = tree_out(package_name);
		package_versions        = getfield(1,package_versions_struct);
		package_versions(1:2)   = [];
		
		for j=1:size(package_versions,"*")
			this_package_struct                          = package_versions_struct(package_versions(j));
			this_package_struct(field)                   = value;
			package_versions_struct(package_versions(j)) = this_package_struct;
		end
		
		tree_out(package_name) = package_versions_struct;
	
	
	// 3rd case : Specific version of a package
	// -------------------------------------------------------------------------
	
	else
		
		if ~ isfield(tree_out,package_name) then
			error(msprintf(gettext("%s: The package ''%s'' is not present in the struct.\n"),"atomsDESCRIPTIONaddField",package_name));
		end
		
		package_versions_struct = tree_out(package_name);
		
		if ~ isfield(package_versions_struct,package_version) then
			error(msprintf(gettext("%s: The version ''%s'' of the package ''%s'' is not present in the struct.\n"),"atomsDESCRIPTIONaddField",package_version,package_name));
		end
		
		this_package_struct                      = package_versions_struct(package_version);
		this_package_struct(field)               = value;
		package_versions_struct(package_version) = this_package_struct;
		tree_out(package_name)                   = package_versions_struct;
		
	end
	
endfunction
