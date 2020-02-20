// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = scatter(varargin)

    polyLine = 0;
    [lhs,rhs] = argn(0);

    // Example:
    if ~rhs
        r = gca().axes_bounds; delete(gca()); xsetech(r);
        t = linspace(0, 25, 200);
        x = t .* cos(t);
        y = t .* sin(t);
        polyLine = scatter(x, y, t, t, "fill", "markerEdgeColor", "darkblue")
        if lhs > 0
            varargout = list(polyLine);
        end
        return
    end

    // Minimal arguments checking to call scatter3(), and call it.
    msg = _("%s: Wrong number of input arguments: At least %d expected.\n");
    if type(varargin(1))==9 then    // graphic handle
        if rhs < 3
            error(msprintf(msg, "scatter", 3));
        end
        _from_Scatter_ = 1  // calling flag = shift of argins (for argins ranks in error msgs)
        polyLine = scatter3d(varargin(1:3), [], varargin(4:$));
    else
        if rhs < 2
            error(msprintf(msg, "scatter", 2))
        end
        _from_Scatter_ = %eps/4
        polyLine = scatter3d(gca(), varargin(1:2), [], varargin(3:$));
    end
    if lhs > 0
        varargout = list(polyLine);
    end
endfunction
