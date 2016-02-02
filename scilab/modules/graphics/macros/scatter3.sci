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

function polyLine = scatter3(varargin)

    polyLine = 0;
    [lhs,rhs] = argn(0);

    if ~rhs
        clf;
        z = linspace(0,25,200);
        x = z.*cos(z);
        y = z.*sin(z);
        polyLine = scatter3(x,y,z,z,z,"fill","markerEdgeColor","darkblue");
        set(gca(),"rotation_angles",[60,45])
        return;
    end

   //detect and set the current axes now:
    if type(varargin(1)) == 9 then // graphic handle
        hdle = varargin(1);
        if hdle.type == "Axes" then
            if size(varargin) < 4 then
                warning("Not enough input arguments.")
                return;
            else
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
    
    if ( isempty(Z) ) then
        if (~isvector(X) | ~isvector(Y) | size(X) ~= size(Y)) then
            warning("X and Y must be vectors of the same length.")
            return;
        end
    else
        if (~isvector(X) | ~isvector(Y) | ~isvector(Z) | or(size(X) ~= size(Y)) | or(size(X) ~= size(Z))) then
            warning("X, Y and Z must be vectors of the same length.")
            return;
        end
    end
        
    n = length(X);
    [S,C,thickness,markStyle,markFg,markBg,fill,scanFailed] = scatterScanVargin(varargin,nextArgin,n);
    if (scanFailed) then
        return;
    end
    
    drawlater();

    if isempty(Z) then 
        if isempty(axesHandle) then
            plot(X,Y);
        else
            plot(axesHandle,X,Y);
        end
        currentEntity = gce();
        polyLine = currentEntity.children;
    else
        if isempty(axesHandle) then
            param3d(X,Y,Z);
        else
            set("current_axes",axesHandle)
            param3d(X,Y,Z);
        end
        polyLine = gce();
    end
    
    if polyLine.Type <> "Polyline" then
        warning("Handle should be a Polyline handle.");
        return;
    end

    scatterSetPolyline(polyLine,S,C,thickness,markStyle,markFg,markBg,fill);
    
    if ~isempty(Z) then
        set(gca(),"cube_scaling","on");
        set(gca(),"grid",[1 1 1]);
    end
    
    drawnow();

endfunction

function [S,C,thickness,markStyle,markFg,markBg,fill,scanFailed] = scatterScanVargin(argins,nextArgin,n)

    scanFailed = %F;
    
    // check for size argument
    S = [];
    if  size(argins) >= nextArgin then
        if isempty(argins(nextArgin)) then
            nextArgin = nextArgin + 1;
        else
            if type(argins(nextArgin)) == 1 then
                [n1,n2] = size(argins(nextArgin));
                if (n1 == 1 & n2 == 1) | (n1 == n & n2 == 1) | (n1 == 1 & n2 == n) then
                    S = argins(nextArgin);
                    if iscolumn(S) then
                        S = S.';
                    end
                    nextArgin = nextArgin + 1;
                else
                    warning("S must be a scalar or a vector of the same length as X.");
                    scanFailed = %T;
                    return;
                end
            end
        end
    end

    // check for color argument
    C = [];
    if  size(argins) >= nextArgin then
        if isempty(argins(nextArgin)) then
            nextArgin = nextArgin + 1;
        else
            [n1,n2] = size(argins(nextArgin));
            if type(argins(nextArgin)) == 1 then
                if (n1 == n & n2 == 1) | (n1 == 1 & n2 == n) then
                    C = scatterLinearColorMap(argins(nextArgin));
                    nextArgin = nextArgin + 1;
                elseif n1 == n & n2 == 3 then
                    C = addcolor(argins(nextArgin));            
                    nextArgin = nextArgin + 1;
                else
                    warning("C must be a vector or a matrix of the same length as X.");
                    scanFailed = %T;
                    return;
                end
                if iscolumn(C) then
                    C = C.';
                end
            elseif type(argins(nextArgin)) == 10 then
                if n1 == 1 & n2 == 1 then
                    // check if string specifies a color
                    colorRGB = name2rgb(argins(nextArgin));
                    if ~isempty(colorRGB) then
                        C = addcolor(colorRGB/255);
                        nextArgin = nextArgin + 1;
                    end        
                elseif (n1 == n & n2 == 1) | (n1 == 1 & n2 == n) then
                    C = addcolor(name2rgb(argins(nextArgin))/255);
                    if isempty(C) then
                        warning("Wrong color specified.");
                        scanFailed = %T;
                        return;
                    else
                         nextArgin = nextArgin + 1;
                    end
                end
            end
        end
    end

    // check for "fill" argument
    fill = %F;
    if  size(argins) >= nextArgin then
        if type(argins(nextArgin)) == 10 then
            if argins(nextArgin) == "fill" then
                fill = %T;
                nextArgin = nextArgin + 1;
            end
        end
    end

    // check for marker argument
    markStyle = 9; // default circle
    if  size(argins) >= nextArgin then
        if type(argins(nextArgin)) == 10 then
            ms = getMarkStyle(argins(nextArgin));
            if ms >= 0 & ms <= 14 then
                markStyle = ms;
                nextArgin = nextArgin + 1;
            end
        end
    end

    // check for property-value pairs
    markFg = -1;
    markBg = -1;
    thickness = 1.0; // default
    while  size(argins) >= nextArgin do
        if size(argins) == nextArgin then
            warning("Incorrect number of inputs for property-value pairs.");
            scanFailed = %T;
            return;
        else
            select argins(nextArgin)
            case "marker"
                markStyle = getMarkStyle(argins(nextArgin+1));
                if markStyle == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property marker."]));
                    scanFailed = %T;
                    return;
                end
            case "markerStyle"
                markStyle = getMarkStyle(argin(nextArgin+1));
                if markStyle == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerStyle."]));
                    scanFailed = %T;
                    return;
                end
            case "markerEdgeColor"
                markFg = colorIndex(argins(nextArgin+1));
                if markFg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerEdgeColor."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerForeground"
                markFg = colorIndex(argins(nextArgin+1));
                if markFg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerForeground."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerFaceColor"
                fill = %T;
                markBg = colorIndex(argins(nextArgin+1));
                if markBg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerFaceColor."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerBackground"
                fill = %T;
                markBg = colorIndex(argins(nextArgin+1));
                if markBg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerBackground."]));
                    scanFailed = %T;
                    return;
                end;
            case "linewidth"
                if type(argins(nextArgin+1)) == 1 then
                    thickness = argins(nextArgin+1);
                else
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property linewidth."]));
                    scanFailed = %T;
                    return;
                end 
            case "thickness"
                if type(argins(nextArgin+1)) == 1 then
                    thickness = argins(nextArgin+1);
                else
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property thickness."]));
                    scanFailed = %T;
                    return;
                end
            else
                warning(strcat(["There is no " argins(nextArgin) " property on the Scatter class."]));
                scanFailed = %T;
                return;
            end
        end
        nextArgin = nextArgin + 2;
    end

endfunction

function colorInd = colorIndex(colorSpec)
    colorInd = -1;
    if type(colorSpec) == 10 & size(colorSpec) == [1 1] then
        colorRGB = name2rgb(colorSpec);
        if ~isempty(colorRGB) then
            colorInd = addcolor(colorRGB/255);
            return;
        end
    elseif type(colorSpec) == 1 & (size(colorSpec) == [1 3] | size(colorSpec) == [3 1]) then
        colorInd = addcolor(colorSpec);
        return; 
    end
    warning("Specified string is an invalid color value.");
endfunction

function markStyle = getMarkStyle(name)
    if type(name) ~= 10 then
        markStyle = -1;
    else
        select name
        case "."
            markStyle = 0;
        case "+"
            markStyle = 1;
        case "x"
            markStyle = 2;
        case "circle plus"
            markStyle = 3;
        case "filled diamond"
            markStyle = 4;
        case "d"
            markStyle = 5;
        case "diamond"
            markStyle = 5;
        case "^"
            markStyle = 6;
        case "v"
            markStyle = 7;
        case "diamond plus"
            markStyle = 8;
        case "o"
            markStyle = 9;
        case "*"
            markStyle = 10;
        case "s"
            markStyle = 11;
        case "square"
            markStyle = 11;
        case ">"
            markStyle = 12;
        case "<"
            markStyle = 13;
        case "p"
            markStyle = 14;
        case "pentagram"
            markStyle = 14;
        else
            markStyle = -1;
        end
    end
endfunction

function colorIndex = scatterLinearColorMap(colorValue)
    cMin = min(colorValue);
    cMax = max(colorValue);
    cmap = get(gcf(),"color_map");
    numColors = size(cmap,1);
    if (cMax-cMin > %eps) then
        colorIndex = (numColors-1)*(colorValue - cMin)/(cMax - cMin) + 1;
    else
        colorIndex = 1; // default color index
    end
endfunction

function scatterSetPolyline(polyLine,S,C,thickness,markStyle,markFg,markBg,fill)

    // set mark mode
    polyLine.line_mode = "off";
    polyLine.mark_mode = "on";

    // set thickness
    polyLine.thickness = thickness;

    // set mark style
    polyLine.mark_style = markStyle;

    // set mark size
    polyLine.mark_size_unit = "point";
    if isempty(S) then
        polyLine.mark_size = 7;
    else
        if size(S) == [1 1] | size(S) == [1 n]
            polyLine.mark_size = ceil(sqrt(4*S/%pi));
        else
            warning("S must be a scalar or a vector of the same length as X.");
            return;
        end
    end

    // set mark foreground and background color
    if isempty(C) then
        if markFg == -1 then
            markFg = addcolor(name2rgb("blue")/255); // default
        end
        polyLine.mark_foreground = markFg;
        if markBg == -1 then
            markBg = markFg;
        end
        if fill == %T then
            polyLine.mark_background = markBg;
        else
            polyLine.mark_background = 0; // transparent
        end
     else
        if size(C) == [1 1] then
            polyLine.mark_foreground = C; 
            if fill == %T then
                polyLine.mark_background = C;
            else
                if markBg == -1 then
                    polyLine.mark_background = 0; // transparent
                else
                    polyLine.mark_background = markBg;
                end
            end
        else
            if fill == %T then
                if markFg == -1 then
                    polyLine.mark_foreground = C; // transparent
                else
                    polyLine.mark_foreground = markFg;
                end
                polyLine.mark_background = C;
            else
                polyLine.mark_foreground = C;
                if markBg == -1 then
                    polyLine.mark_background = 0; // transparent
                else
                    polyLine.mark_background = markBg;
                end
            end
        end
    end

endfunction
