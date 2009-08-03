// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Liste des versions installée de la toolbox "name"

function res = atomsGetInstalledVers(name,allusers)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledVers",1,2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(name) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A Single String expected.\n"),"atomsGetInstalledVers",1));
	end
	
	if size(name,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A Single String expected.\n"),"atomsGetInstalledVers",1));
	end
	
	// allusers management
	// =========================================================================
	
	if rhs < 2 then
		allusers = %T;
	end
	
	// Get the list of installed packages
	// =========================================================================
	packages = atomsGetInstalled(allusers);
	
	// Filter on names
	// =========================================================================
	res = packages(find(packages(:,1) == name),2);
	
	// Descending Sort 
	// =========================================================================
	res = atomsVersionSort( res , "DESC" );
	
endfunction
