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
	
	//Warning which indicate that xgetfile is obsolete
	warnobsolete('uigetfile','5.2');	
	
	
	//Input arguments checking
	if rhs < 0 & rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xgetfile",1,2,3));
		return;
	end

		
	//xgetfile without input
	if rhs == 0 then
		myFile = uigetfile();	
		return;
	end
		
	// 1 input : filemask OR title
	if rhs == 1 then
		if (exists('fileMask') == 1) then
			if (type(fileMask) == 10) then
				myFileMask = fileMask;
				myInitialDir = "";
				myBoxTitle = "Uigetfile";	
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",1));
				return;
			end
		elseif (exists('title', 'local')) then
				myFileMask = "";
				myInitialDir = "";
				myBoxTitle = string(title);	
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",1));
			return;
		return;
		end
	end
	
	// 2 inputs : filemask, initial directory OR filemask, title
	if rhs == 2 then
		if (type(fileMask) == 10 & exists('initialDir') == 1) then
			if type(initialDir) == 10 then
				myFileMask = fileMask;
				myInitialDir = initialDir;
				myBoxTitle = "Uigetfile";	
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",2));
				return;
			end
		elseif (exists('title', 'local')) then
				myFileMask = fileMask;
				myInitialDir = "";
				myBoxTitle = string(title);	
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",1));
			return;
		end
	end
	
	// 3 inputs : filemask, initial directory, box title OR filemask, initial directory, title
	if rhs == 3 then
		if (type(fileMask) == 10 & exists('initialDir') == 1 & exists('boxTitle') == 1) then
			myFileMask = fileMask;
			
			if type(initialDir) == 10 then
				myInitialDir = initialDir;
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",2));
			end
			
			if type(boxTitle) == 10 then
				myBoxTitle = boxTitle;
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",3));
			end
			
		elseif (exists('title', 'local')) then
				myFileMask = fileMask;
				myInitialDir = initialDir;
				myBoxTitle = string(title);	
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetfile",1));
			return;
		end
	end

	//calling uigetfile
	myFile = uigetfile(myFileMask, myInitialDir, myBoxTitle);
	
endfunction