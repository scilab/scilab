// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmap = oceancolormap(varargin)
	
	//rainbowcolormap : Black to White though blue
	
  // Check number of input argument
  if size(varargin)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "oceancolormap", 1));
  end
  n=varargin(1);

  // Check size of input argument
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"oceancolormap",1));
	end

  // Check value of input argument
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"oceancolormap",1,3));
	end

	r = [ 0/3  1/3  2/3  3/3 ; 0    0    0    1 ];
	g = [ 0/3  1/3  2/3  3/3 ; 0    0   1/2   1 ];
	b = [ 0/3  1/3  2/3  3/3 ; 0   1/3  2/3   1 ];
	
	d = 0.5/n;
	
	x = linspace(d,1-d,n)
	cmap = [interpln(r, x)   ;...
			interpln(g, x)   ;...
			interpln(b, x)   ]'   ;
	
	cmap = min(1, max(0 , cmap))  // normaly not necessary
	
endfunction
