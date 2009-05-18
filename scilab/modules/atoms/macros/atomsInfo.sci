// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Display of the toolbox information

function desc = atomsInfo(name)
	
	rhs  = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsInfo",1,2));
	end
	
	// Check input arguments dimensions
	// =========================================================================
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsInfo",1));
	end
	
	if rhs>1 & size(version,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"atomsInfo",2));
	end
	
	// Default value for version
	// =========================================================================
	if rhs<2 then
		version = [];
	end
	
	// we remove the special characters
	// =========================================================================
	normalized_name = atomsNormalizeName(name);
	
	// We try to find the information locally
	// =========================================================================
	
	if atomsIsInstalled(name,version) then
		desc = atomsReadDesc( pathconvert(atomsGetInstalledPath(name,version)+"/DESCRIPTION",%F) );
	end
	
	// We try to find the information on the net
	// =========================================================================
	
	
	
	







endfunction
