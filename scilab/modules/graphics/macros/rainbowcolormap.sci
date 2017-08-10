// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function cmap = rainbowcolormap(varargin)

    //rainbowcolormap : red through orange, yellow, green, blue to violet

    // Check number of input argument
    if size(varargin)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "rainbowcolormap", 1));
    end
    n=varargin(1);

    // Check type of input argument
    // Check if input argument is real
    if typeof(n)<>"constant" | ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real scalar expected.\n"), "rainbowcolormap", 1));
    end

    // Check size of input argument
    if size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a real scalar expected.\n"), "rainbowcolormap", 1));
    end

    if n==0 then
        cmap = [];
        return
    end

    r = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 1   1   1  1/2  0   0   0   0   0  1/2  1];
    g = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 0  1/2  1   1   1   1   1  1/2  0   0   0];
    b = [ 0/10 1/10 2/10 3/10 4/10 5/10 6/10 7/10 8/10 9/10 10/10 ; 0   0   0   0   0  1/2  1   1   1   1   1];

    d = 0.5/max(n,1);

    x = linspace(d,1-d,n)
    cmap = [ interpln(r, x);...
    interpln(g, x);...
    interpln(b, x) ]';
    cmap = min(1, max(0 , cmap))  // normaly not necessary

endfunction
