// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return  1 if version_1 is superior than version_2
// Return -1 if version_2 is superior than version_1
// Return  0 if version_1 and version_2 are equal

function result = atomsVersionCompare( version_1 , version_2 )
	
	rhs    = argn(2);
	result = 0;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsVersionCompare",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(version_1) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsVersionCompare",1));
	end
	
	if type(version_2) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsVersionCompare",2));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(version_1,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A Single String expected.\n"),"atomsVersionCompare",1));
	end
	
	if size(version_1,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A Single String expected.\n"),"atomsVersionCompare",2));
	end
	
	// Check input parameters values
	// =========================================================================
	
	if (length(version_1) > 1) & (regexp(version_1,"/^[0-9]([0-9\.])*[0-9]$/") == []) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",1));
	end
	
	if (length(version_1) == 1) & (regexp(version_1,"/^[0-9]$/") == []) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",1));
	end
	
	if (length(version_2) > 1) & (regexp(version_2,"/^[0-9]([0-9\.])*[0-9]$/") == []) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",2));
	end	
	
	if (length(version_2) == 1) & (regexp(version_2,"/^[0-9]$/") == []) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",2));
	end
	
	// Now : action
	// =========================================================================
	
	if regexp(version_1,"/\./","o") == [] then
		version_1_mat = strtod(version_1);
	else
		version_1_mat = strtod(strsubst( strsplit(version_1,strindex(version_1,".")) , "." , "" ));
	end
	
	if regexp(version_2,"/\./","o") == [] then
		version_2_mat = strtod(version_2);
	else
		version_2_mat = strtod(strsubst( strsplit(version_2,strindex(version_2,".")) , "." , "" ));
	end
	
	version_1_mat_size = size(version_1_mat,"*");
	version_2_mat_size = size(version_2_mat,"*");
	
	for i=1:version_1_mat_size
		
		if i > version_2_mat_size then
			if version_1_mat(i) > 0 then
				// Version_1 is superior than version_2
				result = 1;
				return;
			end
			continue;
		end
		
		if version_1_mat(i) > version_2_mat(i) then
			// Version_1 is superior than version_2
			result = 1;
			return;
		end
		
		if version_2_mat(i) > version_1_mat(i) then
			// Version_2 is superior than version_1
			result = -1;
			return;
		end
		
	end
	
endfunction
