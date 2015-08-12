// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function scatter(varargin)

    [lhs,rhs] = argn(0);

    if ~rhs
        clf;
        t = linspace(0,25,200);
        x = t.*cos(t);
        y = t.*sin(t);
        scatter(x,y,t,t,"fill","markerEdgeColor","darkblue")
        return;
    end

   //detect and set the current axes now:
    if type(varargin(1)) == 9 then // graphic handle
        hdle = varargin(1);
        if hdle.type == "Axes" then
            if size(varargin) < 3 then
                warning("Not enough input arguments.")
                return;
            else
                 disp("Graphic handle specified!")
                axesHandle = varargin(1);
                X = varargin(2);
                Y = varargin(3);
                nextArgin = 4;
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
            axesHandle = [];
            X = varargin(1);
            Y = varargin(2);
            nextArgin = 3;
        end
    end

    if (isempty(X) & isempty(Y)) then
        // nothing has to be done
        return;
    end
    
    if (~isvector(X) | ~isvector(Y) | size(X) ~= size(Y)) then
        warning("X and Y must be vectors of the same length.")
        return;
    end
    
    n = length(X);
    [S,C,thickness,markStyle,markFg,markBg,fill,scanFailed] = scatterScanVargin(varargin,nextArgin,n);
    if (scanFailed) then
        return;
    end

    drawlater();
 
    if isempty(axesHandle) then
       plot(X,Y);
    else
       plot(axesHandle,X,Y);
    end
    
    currentEntity = gce();
    polyLine = currentEntity.children;
    if polyLine.Type <> "Polyline" then
        warning("Handle should be a Polyline handle.");
        return;
    end

    scatterSetPolyline(polyLine,S,C,thickness,markStyle,markFg,markBg,fill);
    
    drawnow();

endfunction
