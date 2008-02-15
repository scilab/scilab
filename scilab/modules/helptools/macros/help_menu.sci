
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function help_menu(i)
	//function associated with the help button in Scilab main window
	browsehelp=browsehelp;// load browsehelp subfunctions
	change_old_man()
	INDEX=make_help_index()
	if i==1 then
		browsehelp(INDEX,"index");
	elseif  i==2 then
		key=x_dialog('Select a keyword','')
		if key==[] then return,end
		key=stripblanks(key(1))
		if key=='' then return,end
		apropos(key)
	elseif   i==3 then
		browsehelp_configure('set')
	end
endfunction
