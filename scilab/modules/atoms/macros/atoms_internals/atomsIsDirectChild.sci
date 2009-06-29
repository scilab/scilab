// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Returns True if the package named "child_name" (version doesn't matter)
// is directy needed by the package whose name is "parent_name" and version is
// "parent_version". No matter whether if "parent_name - parent_version" is
// installed or not

// Input arguments :
//
//   parent_name : . String Array
//                 . mandatory
//
//   parent_version : . String Array
//                    . mandatory
//
//   child_name     : . Single String
//                    . mandatory

// Output arguments :
//
//   result : . Boolean Array

function result = atomsIsDirectChild(parent_name,parent_version,child_name)
	
	rhs    = argn(2);
	result = [];
	
	// Check number of input arguments
	// =========================================================================
	if rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsDirectChild",4));
	end
	
	// Check input parameter type
	// =========================================================================
	
	if type(parent_name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsIsDirectChild",1));
	end
	
	if type(parent_version) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsIsDirectChild",2));
	end
	
	if type(child_name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsIsDirectChild",3));
	end
	
	// Check input parameter dimension
	// =========================================================================
	
	if size(parent_name,"*") <> size(parent_version,"*") then
		error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsIsDirectChild",1,2));
	end
	
	if size(child_name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A Single String expected.\n"),"atomsIsDirectChild",3));
	end
	
	// Loop on parents
	// =========================================================================
	
	for i=1:size(parent_name,"*")
		parent_deps = atomsToolboxDetails(parent_name(i),parent_version(i),"Depends");
		result = [ result ; grep(parent_deps,"/\s"+child_name+"\s/","r") <> [] ];
	end
	
	// reshape the matrix
	// =========================================================================
	result = matrix(result,size(parent_name) );
	
endfunction
