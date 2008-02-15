
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_disablemenus(men)
	
	[lhs,rhs]=argn();
	
	f   = gcf();
	win = f.figure_id;
	
	if rhs<1 then men=EGdata.Menus;end
	
	for k=1:size(men)
		unsetmenu(win,men(k)(1))
	end
	
endfunction
