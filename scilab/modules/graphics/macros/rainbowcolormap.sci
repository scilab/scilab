// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmap = rainbowcolormap(n)
	
	//rainbowcolormap : red through orange, yellow, green, blue to violet
	
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"rainbowcolormap",1));
	end
	
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"rainbowcolormap",1,3));
	end
	
   r = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 1   1   1  1/2  0   0   0   0   0  1/2  1];
   g = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 0  1/2  1   1   1   1   1  1/2  0   0   0];
   b = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 0   0   0   0   0  1/2  1   1   1   1   1];

   d = 0.5/n;

   x = linspace(d,1-d,n)
   cmap = [ interpln(r, x);...
	    interpln(g, x);...
	    interpln(b, x) ]';
   cmap = min(1, max(0 , cmap))  // normaly not necessary

endfunction
