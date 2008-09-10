// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xmltohtml(dirs,titles,directory_language,default_language)
	
	lhs=argn(1);
	rhs=argn(2);
	
	select rhs
	
	case 0 then
		xmltoformat("html");
	case 1 then
		xmltoformat("html",dirs);
	case 2 then
		xmltoformat("html",dirs,titles);
	case 3 then
		xmltoformat("html",dirs,titles,directory_language);
	case 4 then
		xmltoformat("html",dirs,titles,directory_language,default_language);
	else
		error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"),"xmltohtml",4));
	
	end
	
endfunction
