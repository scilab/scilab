// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Remove a toolbox from the list of registered toolboxes

function atomsUnregisterToolbox(name)
	
	if fileinfo(TMPDIR+"/loaded_toolboxes") <> [] then
		load(TMPDIR+"/loaded_toolboxes","loaded_toolboxes");
	else
		return;
	end
	
	toolbox_num = find( loaded_toolboxes == name );
	
	if toolbox_num<>[] then
		loaded_toolboxes(toolbox_num,:)   = []; // delete the name
	end
	
	save(TMPDIR+"/loaded_toolboxes",loaded_toolboxes);
	
endfunction
