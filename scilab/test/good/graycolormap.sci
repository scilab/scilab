// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function cmap = graycolormap(varargin)
//graycmap   linear gray-scale color map.
//r=g=b

  // Check number of input argument
  if size(varargin)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "graycolormap", 1));
  end
  n=varargin(1);

  // Check size of input argument
	if size(n,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"graycolormap",1));
	end

  // Check value of input argument
	if n<3 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer greater or equal than %d expected.\n"),"graycolormap",1,3));
	end

cmap=((0:n-1)'/(n-1))*[1 1 1];
endfunction
