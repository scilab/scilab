// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function atomsRepositorySetOfl(url)
	
	// Check number of input arguments
	// =========================================================================
	rhs = argn(2);
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositorySetOfl",1));
	end
	
	// Check URLs specified as first input argument
	// =========================================================================
	
	if type(url) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single String expected.\n"),"atomsRepositorySetOfl",1));
	end
	
	if size(url,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single String expected.\n"),"atomsRepositorySetOfl",1));
	end
	
	valid_url_pattern    = "/^((((H|h)(T|t)|(F|f))(T|t)(P|p)((S|s)?))\:\/\/)?(www.|[a-zA-Z0-9].)[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\:[0-9]{1,5})*(\/($|[a-zA-Z0-9\.\,\;\?\''\\\+&amp;%\$#\=~_\-]+))*$/";
	
	if ~ regexp(url,valid_url_pattern,"o") == 1 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' is not a valid URL.\n"),"atomsRepositorySetOfl",1,url));
	end
	
	// Overwrite "repositories" file
	// =========================================================================
	mputl(url,pathconvert(SCI+"/modules/atoms/etc/repositories",%F));
	
endfunction
