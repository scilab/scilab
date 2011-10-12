// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cmap = springcolormap(varargin)

    //springcolormap : Magenta to yellow.

    // Check number of input argument
    if size(varargin)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "springcolormap", 1));
    end
    n=varargin(1);

    // Check type of input argument
    if typeof(n)<>"constant" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer value expected.\n"), "springcolormap", 1));
    end

    // Check if input argument is real
    if ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer value expected.\n"), "springcolormap", 1));
    end

    // Check size of input argument
    if size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer value expected.\n"), "springcolormap", 1));
    end

    if n==0 then
        cmap = [];
        return
    end

    red    = ones(n,1);
    green  = (0:n-1)'/max(n-1,1);
    blue   = 1 - green;

    cmap = [red green blue];

endfunction
