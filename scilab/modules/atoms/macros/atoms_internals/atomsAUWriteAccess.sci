// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function res = atomsAUWriteAccess()
	
	res = %F;
	
	try
		mputl("dummy",pathconvert(SCI+"/contrib/dummy",%F));
		mdelete(pathconvert(SCI+"/contrib/dummy",%F));
		res = %T;
	catch
	end
	
endfunction
