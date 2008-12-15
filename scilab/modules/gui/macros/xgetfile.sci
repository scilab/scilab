// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function myFile=xgetfile(fileMask, initialDir, boxTitle)

	[lhs,rhs]=argn(0);
	
	//Input arguments checking
	if rhs <> 1 & rhs <> 2 & rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xgetfile",1,2,3));
		return;
	end
	
	//first argument checking
	if type(fileMask) <> 10 | fileMask == "" then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",1));
		return;
	end
	
	//second argument checking
	if type(initialDir) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",2));
		return;
	end
	
	//third argument checking
	if type(boxTitle) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",3));
		return;
	end
	
	warnobsolete('uigetfile','5.2');
	
	myFile = uigetfile(fileMask, initialDir, boxTitle);
	
endfunction