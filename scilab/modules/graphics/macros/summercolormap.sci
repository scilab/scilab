// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmap = summercolormap(n)
	
	//summercolormap : Green to yellow.
	
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"summercolormap",1));
	end
	
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"summercolormap",1,3));
	end
	
	red    = (0:n-1)'/max(n-1,1);
	green  = (1+red)/2;
	blue   = 0.4*ones(n,1);
	
	cmap = [red green blue];
	
endfunction
