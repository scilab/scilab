// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function clf(varargin)

    nbArg = size(varargin);

    if nbArg==0 then
        h=gcf()
        job="clear"
    elseif nbArg==1 then
        if type(varargin(1))==1 then // win num given
            num=varargin(1)

            h=[];for k=1:size(num,"*"),h=[h;get_figure_handle(num(k))];end

            job="clear"
        elseif type(varargin(1))==9 then // handle given
            h=varargin(1);job="clear"
        elseif type(varargin(1))==10 then // job given
            h=gcf()
            job=varargin(1)
        end
    elseif nbArg==2 then
        if type(varargin(1))==1 then // win num given
            num=varargin(1)

            h=[];for k=1:size(num,"*"),h=[h;get_figure_handle(num(k))];end

        elseif type(varargin(1))==9 then // handle given
            h=varargin(1);
        end
        job=varargin(2);
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "clf", 0, 2));
    end

    if and(job<>["clear","reset"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''clear'' or ''reset'' expected."), "clf", nbArg));
    end

    nbHandles = size(h,"*");

    if nbHandles == 0 then
        return;
    end

    // check that all the handles are figures
    for k=1:nbHandles
        curFig = h(k);
        if curFig.type <> "Figure" & (curFig.type <> "uicontrol" | curFig.style <> "frame") then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A vector of ''Figure'' or ''Frame'' handle expected."), "clf", 1));
        end
    end

    // delete childrens
    for k=1:nbHandles
        curFig = h(k)
        if curFig.type == "uicontrol" then
            haveAxes = %F;
            for kChild = 1:size(curFig.children, "*")
                if curFig.children(kChild).type=="Axes" then
                    haveAxes = %T;
                    break
                end
            end
            delete(curFig.children);
            if haveAxes then
                newaxes(curFig);
            end
        else
            // drawlater
            immediateMode = curFig.immediate_drawing;
            curFig.immediate_drawing = "off";

            delete(curFig.children);

            // drawnow
            curFig.immediate_drawing = immediateMode;

            curFig.info_message = "";
        end
    end


    // reset figures to default values if needed
    if (job == "reset") then
        defaultFig = gdf();
        for k = 1: nbHandles
            curFig = h(k);
            if curFig.type == "uicontrol" then
                continue;
            end

            // drawlater
            immediateMode = curFig.immediate_drawing;
            curFig.immediate_drawing = "off";

            // properties to set
            defaultProps=["figure_position",
            "axes_size",
            "auto_resize",
            "viewport",
            "figure_name",
            "color_map",
            "info_message",
            "pixel_drawing_mode",
            "immediate_drawing",
            "background",
            "visible",
            "rotation_style",
            "event_handler",
            "event_handler_enable",
            "user_data",
            "tag"];

            for i = 1:size(defaultProps,"*")
                defaultValue = get(defaultFig, defaultProps(i));
                if (defaultProps(i) <> "figure_position" | defaultValue <> [-1,-1]) then
                    // don't reset figure pos is defaultValue is [-1,-1]
                    set(curFig, defaultProps(i), defaultValue);
                end

            end

            // drawnow
            curFig.immediate_drawing = immediateMode;
        end
    end

endfunction

