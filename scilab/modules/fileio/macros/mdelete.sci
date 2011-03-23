// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
// Copyright (C) INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mdelete(filename)
	
	// Scilab Project - V. Couvert
	// Delete a file given by filename (or a set of files if meta-characters used in filename)
	
	// Modified by Pierre MARECHAL
	// Scilab Team
	// Date : 2006-06-29
	// Fix the bug2041
	
	// Date : 2007-01-16
	// Fix the bug2288 (Thanks to M. Dubois, http://dubois.ensae.net )
	
	rhs = argn(2);
	if rhs <> 1 then
		error(77);
	end
	
	if type(filename) <> 10 | size(filename,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"),'mdelete'));
	end
	
	// Handle file path
	if getos() == 'Windows' then
		filename = strsubst(filename,filesep(),"/");
	end
	
	// File path
	k = strindex(filename,"/");
	
	if k == [] then
		file_path = "./";
	else
		file_path = part(filename,1:k($));
		filename  = part(filename,k($)+1:length(filename));
	end
	
	if getos() == 'Windows' then
		fullfilename = strsubst(file_path+filename,"/","\");
		lst_files    = listfiles(fullfilename);
	else
		lst_files = listfiles(file_path+filename);
	end
	
	if lst_files<>[] then
		for i=1:size(lst_files,'*')
			 deletefile(lst_files(i));
		end
	end
	
endfunction
