// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function scatter3(varargin)

    [lhs,rhs] = argn(0);

    if ~rhs
        //
        // add example ???
        //
    end

   //detect and set the current axes now:
    if type(varargin(1)) == 9 then // graphic handle
        hdle = varargin(1);
        if hdle.type == "Axes" then
            if size(varargin) < 4 then
                warning("Not enough input arguments.")
                return;
            else
                 disp("Graphic handle specified!")
                axesHandle = varargin(1);
                X = varargin(2);
                Y = varargin(3);
                Z = varargin(4);
                nextArgin = 5;
            end
        else
            warning("Handle should be an Axes handle.")
            return;
        end
    else
        if size(varargin) < 3 then
            warning("Not enough input arguments.")
            return;
        else
            axesHandle = [];
            X = varargin(1);
            Y = varargin(2);
            Z = varargin(3);
            nextArgin = 4;
        end
    end

    if (isempty(X) & isempty(Y) & isempty(Z)) then
        // nothing has to be done
        return;
    end
    
    if (~isvector(X) | ~isvector(Y) | ~isvector(Z) | size(X) ~= size(Y) | size(X) ~= size(Z)) then
        warning("X, Y and Z must be vectors of the same length.")
        return;
    end
    
    n = length(X);
    [S,C,thickness,markStyle,markFg,markBg,fill,scanFailed] = scatterScanVargin(varargin,nextArgin,n);
    if (scanFailed) then
        return;
    end
    
    drawlater();
 
    if isempty(axesHandle) then
       param3d(X,Y,Z);
    else
       set("current_axes",axesHandle)
       param3d(X,Y,Z);
    end
    
    polyLine = gce();
    if polyLine.Type <> "Polyline" then
        warning("Handle should be a Polyline handle.");
        return;
    end

    scatterSetPoyline(polyLine,S,C,thickness,markStyle,markFg,markBg,fill);
    
    set(gca(),"cube_scaling","on");
    set(gca(),"grid",[1 1 1]);
    
    drawnow();

endfunction

