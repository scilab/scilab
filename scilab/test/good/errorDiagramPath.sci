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

function ret = errorDiagramPath(path, highlighted_blocks, msg, field, show_intermediate, close_opened)

	[lhs, rhs] = argn(0);
	if rhs < 3 then
		error("%s: argument error", errorDiagramPath);
	end
	
	if rhs < 4 then
		field = "";
	end

	if rhs < 5 then
		show_intermediate = %t;
	end

	if rhs < 6 then
		close_opened = %t;
	end

	ret = [];

	//3 steps ( open diagrams, show msg, close diagrams )
	//1st : open path diagram
	openDiagramPath(path, highlighted_blocks, show_intermediate);
	
	//2nd : show message
	
	//if msg is empty, no show dialog
	if msg <> [] & msg <> "" then
		if field == [] | field == "" then
			message(msg);
		else
			ret = evstr(dialog(msg, field));
		end
	end
	
	closeDiagramPath(path, highlighted_blocks, close_opened);
endfunction
