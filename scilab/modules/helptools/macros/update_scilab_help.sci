
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// update modules help chapters

function update_scilab_help()
	clearglobal %helps_modules
	global %helps_modules
	modules=getmodules();
	index=size(modules);
	for i=1:index(1) do 
		add_module_help_chapter(modules(i));
	end
	make_help_index()
	clear modules index i
endfunction
