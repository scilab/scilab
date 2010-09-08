// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function add_module_help_chapter(modulename)
	
	if and(getscilabmode() <> ["NW";"STD"]) then
		return;
	end
	
	path_addchapter = SCI+"/modules/"+modulename+"/help/"+getlanguage()+"/addchapter.sce";
	
	if ~isfile(path_addchapter) then
		path_addchapter = SCI+"/modules/"+modulename+"/help/"+getdefaultlanguage()+"/addchapter.sce";
	end
	
	if isfile(path_addchapter) then
	  exec(path_addchapter);
	end
	
endfunction
