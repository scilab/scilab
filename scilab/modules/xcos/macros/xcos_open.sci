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

function xcos_open(blockId, show)

	[lhs, rhs] = argn(0);
	if rhs == 1 then
		show = %t;
	end    

	if size(scs_m.objs(blockId).doc) > 0 then
		xcosDiagramOpen(scs_m.objs(blockId).doc(1), show);
	end
endfunction
