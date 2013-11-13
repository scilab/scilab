// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function cmap = graycolormap(varargin)
    //graycmap   linear gray-scale color map.
    //r=g=b

    // Check number of input argument
    if size(varargin)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "graycolormap", 1));
    end
    n=varargin(1);

    // Check type of input argument
    // Check if input argument is real
    if typeof(n)<>"constant" | ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real scalar expected.\n"), "graycolormap", 1));
    end

    // Check size of input argument
    if size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a real scalar expected.\n"), "graycolormap", 1));
    end

    cmap=((0:n-1)'/max(n-1,1))*[1 1 1];
endfunction
