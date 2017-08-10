// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function polyLine = scatter(varargin)

    polyLine = 0;
    [lhs,rhs] = argn(0);

    if ~rhs
        clf;
        t = linspace(0,25,200);
        x = t.*cos(t);
        y = t.*sin(t);
        polyLine = scatter(x,y,t,t,"fill","markerEdgeColor","darkblue")
        return;
    end

    //detect and set the current axes now:
    n = size(varargin);
    if type(varargin(1)) == 9 then // graphic handle
        hdle = varargin(1);
        if hdle.type == "Axes" then
            if n < 3 then
                warning("Not enough input arguments.")
                return;
            else
                axesHandle = varargin(1);
                X = varargin(2);
                Y = varargin(3);
                polyLine = scatter3(axesHandle,X,Y,[],varargin(4:n));
            end
        else
            warning("Handle should be an Axes handle.")
            return;
        end
    else
        if size(varargin) < 2 then
            warning("Not enough input arguments.")
            return;
        else
            X = varargin(1);
            Y = varargin(2);
            polyLine = scatter3(X,Y,[],varargin(3:n));
        end
    end
    
endfunction
