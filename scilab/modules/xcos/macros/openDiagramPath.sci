//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function ret = openDiagramPath(path, highlighted_blocks, msg, field, show_intermediate)

	//1st : open path diagram
	for i = 1 : size(path, "*")
		//highlight parent block
		xcosShowBlockWarning(path(i))
		//open superblock
		if  i == size(path, "*") then
			xcos_open(path(i), %t);
		else
			xcos_open(path(i), show_intermediate);
		end
		//build new scs_m to go deeper
		scs_m=scs_m.objs(path(i)).model.rpar;
	end
	
	//highlight incriminated blocks
	for i = 1 : size(highlighted_blocks,"*")
		xcosShowBlockWarning(highlighted_blocks(i));
	end
endfunction
