// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2017 - Samuel GOUGEON
//
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
        h = gcf()
        job = "clear"
    elseif nbArg==1 then
        if type(varargin(1))==1 then     // win num given
            num = varargin(1)
            h = [];
            for k = 1:size(num,"*")
                h = [h ; get_figure_handle(num(k))];
            end
            job = "clear"
        elseif type(varargin(1))==9 then // handle given
            h = varargin(1);
            job = "clear"
        elseif type(varargin(1))==10 then // job given
            h = gcf()
            job = varargin(1)
        end
    elseif nbArg==2 then
        if type(varargin(1))==1 then     // win num given
            num = varargin(1)
            h = [];
            for k = 1:size(num,"*")
                h = [h ; get_figure_handle(num(k))];
            end
        elseif type(varargin(1))==9 then // handle given
            h = varargin(1);
        end
        job = varargin(2);
    else
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.")
        error(msprintf(msg, "clf", 0, 2))
    end

    if and(job<>["clear","reset"]) then
        msg = _("%s: Wrong value for input argument #%d: ''clear'' or ''reset'' expected.")
        error(msprintf(msg, "clf", nbArg))
    end

    nbHandles = size(h,"*");
    if nbHandles == 0 then
        return
    end

    // check that all the handles are figures
    for k = 1:nbHandles
        curFig = h(k);
        if curFig.type <> "Figure" & (curFig.type <> "uicontrol" | curFig.style <> "frame")
            msg = _("%s: Wrong type for input argument #%d: A vector of ''Figure'' or ''Frame'' handle expected.")
            error(msprintf(msg, "clf", 1))
        end
    end

    // delete childrens
    for k = 1:nbHandles
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
            // Forces drawlater
            immediateMode = curFig.immediate_drawing;
            curFig.immediate_drawing = "off";
            delete(curFig.children);
            curFig.info_message = "";            // Clears the infobar message
            curFig.event_handler_enable = "off"; // Disables the event handler
            // Restores the drawlater entry status:
            curFig.immediate_drawing = immediateMode;
        end
    end

    // reset figures to default values if needed
    if (job == "reset") then
        defaultFig = gdf();
        // This literal list should be replaced with a getproperties(gdf())
        // when such a function will be available:
        allprops = [
        "children"
        "figure_position"
        "figure_size"
        "axes_size"
        "auto_resize"
        "viewport"
        "figure_name"
        "figure_id"
        "info_message"
        "color_map"
        "pixel_drawing_mode"
        "anti_aliasing"
        "immediate_drawing"
        "background"
        "visible"
        "rotation_style"
        "event_handler"
        "event_handler_enable"
        "user_data"
        "resizefcn"
        "closerequestfcn"
        "resize"
        "toolbar"
        "toolbar_visible"
        "menubar"
        "menubar_visible"
        "infobar_visible"
        "dockable"
        "layout"
        "layout_options"
        "default_axes"
        "icon"
        "tag"
        ];
        excluded0 = ["children" "dockable" "menubar" "toolbar" "immediate_drawing"]
        excluded0 = [excluded0 "layout" "layout_options"] // http://bugzilla.scilab.org/14955
        for k = 1: nbHandles
            curFig = h(k);
            if curFig.type == "uicontrol" then
                continue;
            end

            // Forces drawlater mode
            curFig.immediate_drawing = "off";

            // properties to set
            excluded = excluded0
            if isDocked(curFig)            // http://bugzilla.scilab.org/11476
                excluded = [excluded "figure_position" "figure_size" "axes_size"]
            end
            defaultProps = setdiff(allprops, excluded);

            // Settings
            for i = 1:size(defaultProps,"*")
                defaultValue = get(defaultFig, defaultProps(i));
                if (defaultProps(i) <> "figure_position" | defaultValue <> [-1,-1]) then
                    // don't reset figure pos is defaultValue is [-1,-1]
                    set(curFig, defaultProps(i), defaultValue);
                end
            end
            set(curFig, "immediate_drawing", defaultFig.immediate_drawing)
        end
    end
endfunction

function yn = isDocked(fh)
    // http://fileexchange.scilab.org/toolboxes/360000
    sf = size(fh)
    fh = fh(:)
    fs = matrix(fh.figure_size,2,-1)'
    as = matrix(fh.axes_size,2,-1)'

    yn = (fh.dockable=="on" & ((fs(:,1)-as(:,1)) > 20)')
    // A test on vertical dimensions is more complicated, due to switchable
    // menubar, toolbar and infobar.
    yn = matrix(yn, sf)
endfunction

