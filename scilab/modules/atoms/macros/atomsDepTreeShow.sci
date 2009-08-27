// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// internal function

// Input arguments :

//   name : . technical name of the package
//          . single string
//          . mandatory

//   version : . version of the package
//             . single string
//             . optional

// Output arguments :

function atomsDepTreeShow(name,version)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/modules/atoms/macros/atoms_internals/lib");
	end
	
	lhs = argn(1);
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if (rhs < 1) | (rhs > 2) then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDepTreeShow",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDepTreeShow",1));
	end
	
	if (rhs>=2) & (type(version) <> 10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A single string expected.\n"),"atomsDepTreeShow",2));
	end
	
	// Check input parameters dimensions
	// =========================================================================
	
	if size(name) <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDepTreeShow",1));
	end
	
	if (rhs>=2) & (size(name)<>1) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsDepTreeShow",1));
	end
	
	// Get the dependency tree
	// =========================================================================
	
	if rhs>1 then
		tree = atomsDepTreeExt(name,version)
	else
		tree = atomsDepTreeExt(name)
	end
	
	situation = struct();
	situation("current_level")  = 1;
	
	mprintf("\n");
	atomsDispTree(tree,situation)
	
endfunction


function atomsDispTree(tree,situation)
	
	fields      = getfield(1,tree);
	fields(1:2) = [];
	
	current_level = situation("current_level");
	situation("level"+string(current_level)+"_number") = size(fields,"*");
	situation("level"+string(current_level)+"_pos")    = 1;
	
	for i=1:size(fields,"*")
		
		this_package_details = tree(fields(i));
		
		for j=1:current_level-1
			if situation("level"+string(j)+"_pos") < situation("level"+string(j)+"_number") then
				mprintf("|   ");
			else
				mprintf("    ");
			end
		end
		
		if current_level == 1 then
			mprintf("    ");
		elseif situation("level"+string(current_level)+"_pos") == situation("level"+string(current_level)+"_number") then
			mprintf("`-- ");
		else
			mprintf("|-- ");
		end
		
		mprintf("%s - %s\n",this_package_details("Toolbox"),this_package_details("Version"));
		
		if isfield(this_package_details,"DependencyTree") then
			
			next_situation                  = situation;
			next_situation("current_level") = current_level + 1;
			this_package_deptree            = this_package_details("DependencyTree");
			atomsDispTree(this_package_deptree,next_situation);
			
		end
		
		situation("level"+string(current_level)+"_pos") = situation("level"+string(current_level)+"_pos") + 1;
		
	end
	
endfunction
