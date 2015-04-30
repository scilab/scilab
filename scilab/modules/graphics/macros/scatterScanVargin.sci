// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [S,C,thickness,markStyle,markFg,markBg,fill,scanFailed] = scatterScanVargin(argins,nextArgin,n)

    scanFailed = %F;
    
    // check for size argument
    // disp("check for size");
    // disp(["nextArgin = " string(nextArgin)]);
    S = [];
    if  size(argins) >= nextArgin then
        if isempty(argins(nextArgin)) then
            // disp("S = []");
            nextArgin = nextArgin + 1;
        else
            if type(argins(nextArgin)) == 1 then
                [n1,n2] = size(argins(nextArgin));
                if (n1 == 1 & n2 == 1) | (n1 == n & n2 == 1) | (n1 == 1 & n2 == n) then
                    // disp("Size specified!");
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
    // disp("check for color");
    // disp(["nextArgin = " string(nextArgin)]);
    C = [];
    if  size(argins) >= nextArgin then
        if isempty(argins(nextArgin)) then
            // disp("C = []");
            nextArgin = nextArgin + 1;
        else
            [n1,n2] = size(argins(nextArgin));
            if type(argins(nextArgin)) == 1 then
                if (n1 == n & n2 == 1) | (n1 == 1 & n2 == n) then
                    // disp("Color vector specified!");
                    C = scatterLinearColorMap(argins(nextArgin));
                    nextArgin = nextArgin + 1;
                elseif n1 == n & n2 == 3 then
                    // disp("Color RGB matrix specified!");
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
                        // disp("Single color string specified!");
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
                         // disp("Color vector of strings specified!");
                         nextArgin = nextArgin + 1;
                    end
                end
            end
        end
    end

    // check for "fill" argument
    // disp("check for fill");
    // disp(["nextArgin = " string(nextArgin)]);
    fill = %F;
    if  size(argins) >= nextArgin then
        if type(argins(nextArgin)) == 10 then
            if argins(nextArgin) == "fill" then
                // disp("fill specified");
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
                // disp(strcat(["marker " argins(nextArgin)]));
                markStyle = ms;
                nextArgin = nextArgin + 1;
            end
        end
    end

    // check for property-value pairs
    // disp("check for property-value pairs");
    // disp(["nextArgin = " string(nextArgin)]);
    markFg = -1;
    markBg = -1;
    thickness = 1.0; // default
    while  size(argins) >= nextArgin do
        if size(argins) == nextArgin then
            warning("Incorrect number of inputs for property-value pairs.");
            scanFailed = %T;
            return;
        else
            // disp(["nextArgin = " string(nextArgin)]);
            select argins(nextArgin)
            case "marker"
                // disp("Property marker");
                markStyle = getMarkStyle(argins(nextArgin+1));
                if markStyle == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property marker."]));
                    scanFailed = %T;
                    return;
                end
            case "markerStyle"
                // disp("Property markerStyle");
                markStyle = getMarkStyle(argin(nextArgin+1));
                if markStyle == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerStyle."]));
                    scanFailed = %T;
                    return;
                end
            case "markerEdgeColor"
                // disp("Property markerEdgeColor");
                markFg = colorIndex(argins(nextArgin+1));
                if markFg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerEdgeColor."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerForeground"
                // disp("Property markerForeground");
                markFg = colorIndex(argins(nextArgin+1));
                if markFg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerForeground."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerFaceColor"
                // disp("Property markerFaceColor");
                fill = %T;
                markBg = colorIndex(argins(nextArgin+1));
                if markBg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerFaceColor."]));
                    scanFailed = %T;
                    return;
                end;
            case "markerBackground"
                // disp("Property markerBackground");
                fill = %T;
                markBg = colorIndex(argins(nextArgin+1));
                if markBg == -1 then
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property markerBackground."]));
                    scanFailed = %T;
                    return;
                end;
            case "linewidth"
                if type(argins(nextArgin+1)) == 1 then
                    // disp("Property linewidth");
                    thickness = argins(nextArgin+1);
                else
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property linewidth."]));
                    scanFailed = %T;
                    return;
                end 
            case "thickness"
                if type(argins(nextArgin+1)) == 1 then
                    // disp("Property thickness");
                    thickness = argins(nextArgin+1);
                else
                    warning(strcat([argins(nextArgin+1) " is not a valid value for property thickness."]));
                    scanFailed = %T;
                    return;
                end
            else
                warning(strcat(["There is no " argin(nextArgin) " property on the Scatter class."]));
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
            // disp("Single color string specified!");
            colorInd = addcolor(colorRGB/255);
            return;
        end
    elseif type(colorSpec) == 1 & (size(colorSpec) == [1 3] | size(colorSpec) == [3 1]) then
        // disp("Color RGB triple specified!");
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
