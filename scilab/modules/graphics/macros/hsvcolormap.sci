// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=hsvcolormap(n)
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"hsvcolormap",1));
	end
	
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"hsvcolormap",1,3));
	end
  c = hsv2rgb([(0:n-1)'/n ones(n,2)]);
endfunction
