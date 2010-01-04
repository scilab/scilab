// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmap = bonecolormap(varargin)
	
	//bonecolormap : Gray colormap with a light blue tone.
	
  // Check number of input argument
  if size(varargin)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "bonecolormap", 1));
  end
  n=varargin(1);

  // Check size of input argument
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"bonecolormap",1));
	end

  // Check value of input argument
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"bonecolormap",1,3));
	end
	
	red_1   = (0:n-1)'/(n-1);
	green_1 = (0:n-1)'/(n-1);
	blue_1  = (0:n-1)'/(n-1);
	
	n1 = fix(3/8*n);
	n2 = n1;
	n3 = n-(n1+n2);
	
	red_2   = [zeros(n1,1) ; zeros(n2,1) ; (1:n3)'/(n3)];
	green_2 = [zeros(n1,1) ; (1:n2)'/n2  ; ones(n3,1)];
	blue_2  = [(1:n1)'/n1  ; ones(n2,1)  ; ones(n3,1)];
	
	cmap = [ ((7*red_1) + red_2)/8 ((7*green_1) + green_2)/8 ((7*blue_1) + blue_2)/8 ];
	
endfunction
