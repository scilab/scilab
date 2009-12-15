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

function closeDiagramPath(path, highlighted_blocks, close_opened)
	//3rd : close path diagrams
	//close opened diagram
	
	if close_opened == %t then
		for k=1:size(path,'*')
			xcos_close(path(k));
			scs_m=scs_m.objs(path(k)).model.rpar;
		end

		//reset scs_m value from calling scope
		clear scs_m;
		
		//remove highlight on block
		if path == [] then
			for i = 1 : size(highlighted_blocks,"*")
				xcosClearBlockWarning(highlighted_blocks(i));
			end
		else
			xcosClearBlockWarning(path(1))
		end
	else
		//disable warnings in diagram path
		for k=1:size(path,'*')
			xcosClearBlockWarning(path(k))
			scs_m=scs_m.objs(path(k)).model.rpar;
		end
		
		//disable warning in the last diagram
		for i = 1 : size(highlighted_blocks,"*")
			xcosClearBlockWarning(highlighted_blocks(i));
		end
	end
	
	
endfunction