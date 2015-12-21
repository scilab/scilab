// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
