// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

