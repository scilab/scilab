// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function atomsDESCRIPTIONwrite(description_in,file_out)
	
	// Check input parameters number
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONwrite",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(description_in) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONwrite",1));
	end
	
	if type(file_out) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONwrite",1));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(file_out,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDESCRIPTIONwrite",2));
	end
	
	// Build the string matrix
	// =========================================================================
	
	str_mat            = [];
	
	package_names      = getfield(1,description_in);
	package_names(1:2) = [];
	
	for i=1:size(package_names,"*")
		
		package_versions_struct = description_in(package_names(i));
		
		if type(package_versions_struct) <> 17 then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: The matrix oriented typed list is not well formatted.\n"),"atomsDESCRIPTIONwrite",1));
		end
		
		package_versions      = getfield(1,package_versions_struct);
		package_versions(1:2) = [];
		
		for j=1:size(package_versions,"*")
			
			str_mat = [ str_mat ; "// =============================================================================" ];
			
			this_package = package_versions_struct(package_versions(j));
			
			this_package_fields      = getfield(1,this_package);
			this_package_fields(1:2) = [];
			
			for k=1:size(this_package_fields,"*")
				
				this_field = this_package_fields(k);
				this_value = this_package(this_package_fields(k));
				
				if type(this_value)<>10 then
					continue;
				end
				
				str_mat = [ str_mat ; sprintf("%s: %s",this_field,this_value(1)) ];
				
				if size(this_value,"*") == 1 then
					continue;
				end
				
				for l=2:size(this_value,"*")
					str_mat = [ str_mat ; sprintf(" %s",this_value(l)) ];
				end
				
			end
			
		end
		
	end
	
	// Put the string matrix in the wanted file
	// =========================================================================
	
	if isempty(str_mat) then
		mdelete(file_out);
	
	elseif ~ mputl(str_mat,file_out) then
		error(msprintf(gettext("%s: The file ''%s'' cannot be written.\n"),"atomsDESCRIPTIONwrite",file_out));
		
	end
	
endfunction
