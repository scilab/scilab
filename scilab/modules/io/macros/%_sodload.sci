// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = %_sodload(%__varnameList__)

    function varValue = %__convertVariable__(varValue, varName)
        if typeof(varValue) == "ScilabMatrixHandle" then
            //convert tlist to handle
            varValue = createMatrixHandle(varValue);
        elseif typeof(varValue) == "ScilabMacro" then
            //convert tlist to macro
            varValue = createMacro(varValue, varName);
        elseif isList(varValue) then
            //list container
            varValue = parseList(varValue);
        end
    endfunction

    function result = isList(var)
        //15 : list
        //16 : tlist
        //17 : mlist
        if or(type(var) == [15, 16, 17]) then
            result  = %t;
        else
            result = %f;
        end
    endfunction

    function varValue = parseList(varValue)

        if or(typeof(varValue)==["ce","st"]) then
            if typeof(varValue)=="st" then
                fieldNames = fieldnames(varValue);
            else
                fieldNames = varValue;
            end

            for kField = 1:size(fieldNames,"*")

                if typeof(varValue)=="st" then
                    fieldValue = varValue(fieldNames(kField));
                else
                    fieldValue = varValue{kField};
                end

                if typeof(fieldValue) == "ScilabMatrixHandle" then
                    fieldValue = createMatrixHandle(fieldValue);
                elseif typeof(fieldValue) == "ScilabMacro" then
                    //convert tlist to macro
                    fieldValue = createMacro(fieldValue, "function");
                elseif isList(fieldValue) then
                    fieldValue = parseList(fieldValue);
                end

                if typeof(varValue)=="st" then
                    s = size(varValue, "*");
                    if s > 1 then
                        if typeof(fieldValue) <> "list" then //houston !
                        end

                        for iDim = 1:s
                            varValue(iDim)(fieldNames(kField)) = fieldValue(iDim);
                        end
                    else
                        varValue(fieldNames(kField)) = fieldValue;
                    end
                else
                    varValue{kField} = fieldValue;
                end

            end

        elseif typeof(varValue)=="list" then
            for i = definedfields(varValue)
                if typeof(varValue(i)) == "ScilabMatrixHandle" then
                    varValue(i) = createMatrixHandle(varValue(i));
                elseif typeof(varValue(i)) == "ScilabMacro" then
                    //convert tlist to macro
                    varValue(i) = createMacro(varValue(i), "function");
                elseif isList(varValue(i)) then
                    varValue(i) = parseList(varValue(i));
                else
                    varValue(i) = varValue(i);
                end
            end
        else
            fieldNb = size(getfield(1, varValue), "*");
            for kField = 2:fieldNb // Do not inspect first field (field names)
                fieldValue = getfield(kField, varValue);
                if typeof(fieldValue) == "ScilabMatrixHandle" then
                    fieldValue = createMatrixHandle(fieldValue);
                elseif typeof(fieldValue) == "ScilabMacro" then
                    //convert tlist to macro
                    fieldValue = createMacro(fieldValue, "function");
                elseif isList(fieldValue) then
                    fieldValue = parseList(fieldValue);
                end
                varValue = setfield(kField, fieldValue, varValue);
            end
        end
    endfunction

    function h = createMatrixHandle(matrixHandle)
        h = [];
        if typeof(matrixHandle) <> "ScilabMatrixHandle" then
            return;
        end

        for i = prod(matrixHandle.dims):-1:1

            newItem = createSingleHandle(matrixHandle.values(i));
            if newItem == [] then
                continue;
            end

            h($+1) = newItem;
            if or(fieldnames(matrixHandle.values(i))=="user_data") then // TODO Remove after graphic branch merge
                if isList(matrixHandle.values(i).user_data) then
                    set(h($), "user_data", parseList(matrixHandle.values(i).user_data));
                elseif typeof(matrixHandle.values(i).user_data) == "ScilabMatrixHandle" then
                    set(h($), "user_data", createMatrixHandle(matrixHandle.values(i).user_data));
                end
            end
        end
    endfunction

    function h = createSingleHandle(item)
        select item.type
        case "Figure"
            h = createFigure(item);
        case "Axes"
            h = createAxes(item);
        case "Polyline"
            h = createPolyline(item);
        case "Plot3d"
            h = createPlot3d(item);
        case "Fac3d"
            h = createFac3d(item);
        case "Compound"
            h = createCompound(item);
        case "Rectangle"
            h = createRectangle(item);
        case "Arc"
            h = createArc(item);
        case "Champ"
            h = createChamp(item);
        case "Segs"
            h = createSegs(item);
        case "Grayplot"
            h = createGrayplot(item);
        case "Matplot"
            h = createMatplot(item);
        case "Fec"
            h = createFec(item);
        case "Legend"
            h = createLegend(item);
        case "Text"
            h = createText(item);
        case "Axis"
            h = createAxis(item);
        case "uimenu"
            h = createuimenu(item);
        case "uicontextmenu"
            h = createuicontextmenu(item);
        case "uicontrol"
            h = createuicontrol(item);
        case "Datatip"
            h = createDatatip(item);
        case "Light"
            h = createLight(item);
        else
            error("handle of type " + item.type + " unhandled");
            h = [];
        end
    endfunction

    //
    // FIGURE
    //
    function h = createFigure(figureProperties)
        fields = fieldnames(figureProperties);
        fields(1) = [];

        if or(fields=="resize") then
            if figureProperties.menubar<>"figure" ..
                | figureProperties.toolbar<>"figure" ..
                | figureProperties.dockable<>"on" ..
                | figureProperties.default_axes<>"on" then
                // File created by Scilab 5.5.0 or more
                h = figure("menubar", figureProperties.menubar, ...
                "toolbar", figureProperties.toolbar, ...
                "dockable", figureProperties.dockable, ...
                "default_axes", figureProperties.default_axes, ...
                "visible", "off");
                h.background = -2;
                fields(fields=="menubar") = [];
                fields(fields=="toolbar") = [];
                fields(fields=="dockable") = [];
                fields(fields=="default_axes") = [];
                fields(fields=="visible") = [];
            else
                [lnums, fnames] = where();
                ind = grep(fnames, "xload");
                xload_mode = (ind ~= []);
                if xload_mode then // See bug #3975
                    h = gcf();
                else
                    h = scf();
                end
                h.visible = "off";
            end
        else
            if isempty(winsid()) then
                h = figure("visible", "off");
                h.background = -2;
            else
                h = gcf();
                h.visible = "off";
            end
        end

        // Following propeties will be set after all other ones
        isVisible = figureProperties.visible;
        fields(fields=="visible") = [];
        resizefcn = figureProperties.resizefcn;
        fields(fields=="resizefcn") = [];
        event_handler = figureProperties.event_handler;
        fields(fields=="event_handler") = [];

        // Ignore figure_id
        fields(fields=="figure_id") = [];

        h.figure_position=figureProperties.figure_position;
        fields(fields=="figure_position") = [];
        // set auto_resize first otherwise viewport modification may not have any effect.
        h.auto_resize = figureProperties.auto_resize;
        fields(fields=="auto_resize") = [];
        h.figure_size = figureProperties.figure_size;
        fields(fields=="figure_size") = [];
        // set axes_size last because it's more important than figure_size
        h.axes_size = figureProperties.axes_size;
        fields(fields=="axes_size") = [];

        for i = 1:size(fields, "*")
            if fields(i) == "children" then
                c = figureProperties(fields(i));
                s = prod(c.dims);
                createSingleHandle(c.values(s));
                for  i = s-1:-1:1
                    xsetech(wrect=[0 0 .1 .1])
                    createSingleHandle(c.values(i));
                end
            else
                if fields(i)<>"pixmap" then // See bug #13310
                    set(h, fields(i), figureProperties(fields(i)));
                end
            end
        end

        h.resizefcn = resizefcn;
        h.event_handler = event_handler;
        h.visible = isVisible;
    endfunction

    //
    // LABEL
    //
    function h = createLabel(labelProperties, h)
        fields = fieldnames(labelProperties);
        fields(1) = [];
        for i = 1:size(fields, "*")
            set(h, fields(i), labelProperties(fields(i)));
        end
    endfunction

    //
    // TICKS
    //
    function h = createTicks(ticksProperties)
        h = tlist(["ticks","locations","labels"], [], []);
        fields = fieldnames(ticksProperties);
        for i = 1:size(fields, "*")
            h(fields(i)) = ticksProperties(fields(i));
        end
    endfunction

    //
    // AXES
    //
    function h = createAxes(axesProperties)
        // Hack to determine whether %h_load has been called by the %h_copy macro
        // in which case a new Axes object is created

        [lnums, fnames] = where();
        ind = grep(fnames, "%h_copy");
        if ~isempty(ind) then
            newaxes();
        end;

        h = gca();
        fields = fieldnames(axesProperties);
        fields(1) = [];

        // Get log_flags to be sure to set them after data_bounds
        log_flags = axesProperties.log_flags;
        fields(fields=="log_flags") = [];

        // Get mark_mode to be sure to set it after mark_style
        mark_mode = axesProperties.mark_mode;
        fields(fields=="mark_mode") = [];

        // Get auto_ticks to be sure to set it after ticks labels
        auto_ticks = axesProperties.auto_ticks;
        fields(fields=="auto_ticks") = [];
        automargins = %f;
        if isfield(axesProperties, "auto_margins") then
            auto_margins = axesProperties.auto_margins;
            fields(fields=="auto_margins") = [];
            automargins = %t;
        end

        for i = 1:size(fields, "*")
            if or(fields(i) == ["title","x_label","y_label","z_label"]) then
                createLabel(axesProperties(fields(i)), h(fields(i)));
            elseif or(fields(i) == ["x_ticks", "y_ticks", "z_ticks"]) then
                set(h, fields(i), createTicks(axesProperties(fields(i))));
            elseif fields(i) == "children" then
                createMatrixHandle(axesProperties(fields(i)));
            elseif fields(i) == "clip_state" then
                if axesProperties.clip_state=="on" then
                    set(h,"clip_box",axesProperties.clip_box);
                end
                set(h,"clip_state", axesProperties.clip_state);
            elseif fields(i) == "clip_box" then
                // managed with 'clip_state'
            elseif fields(i) == "data_bounds" then
                set(h, "data_bounds", axesProperties.data_bounds);
                set(h, "log_flags", log_flags);
            elseif fields(i) == "mark_style" then
                set(h, "mark_style", axesProperties.mark_style);
                set(h, "mark_mode", mark_mode);
            else
                set(h, fields(i), axesProperties(fields(i)));;
            end
        end

        set(h, "auto_ticks", auto_ticks);
        if automargins then
            set(h, "auto_margins", auto_margins);
        end

        // Legend management
        global %LEG
        if ~isempty(%LEG) then
            // Get handles from paths
            links=getlinksfrompath(h, %LEG.paths)
            if ~isempty(links) then
                L = captions(links, %LEG.text)
                L.visible         = %LEG.visible
                L.font_style      = %LEG.font_style
                L.font_size       = %LEG.font_size
                L.font_color      = %LEG.font_color
                L.fractional_font = %LEG.fractional_font
                L.mark_mode       = "off";
                L.legend_location = %LEG.legend_location
                L.position        = %LEG.position
                L.line_mode       = %LEG.line_mode
                L.thickness       = %LEG.thickness
                L.foreground      = %LEG.foreground
                L.fill_mode       = %LEG.fill_mode
                L.background      = %LEG.background
                if %LEG.clip_state=="on" then
                    L.clip_box      = %LEG.clip_box
                end
                L.clip_state      = %LEG.clip_state
                L.user_data       = %LEG.user_data
            else
                warning(msprintf(_("%s: Legend does not fit with the current context. Skipped\n"), "load"));
            end
        end
        clearglobal %LEG

    endfunction

    //
    // POLYLINE
    //
    function h = createPolyline(polylineProperties)
        fields = fieldnames(polylineProperties);
        fields(1) = [];

        xpoly(polylineProperties.data(:,1), polylineProperties.data(:,2))

        h = gce();

        if polylineProperties.clip_state=="on" then
            set(h, "clip_box", polylineProperties.clip_box)
        end
        set(h, "clip_state", polylineProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        if polylineProperties.interp_color_mode=="on" & ~isempty(polylineProperties.interp_color_vector) then
            set(h, "interp_color_vector", polylineProperties.interp_color_vector);
            set(h, "interp_color_mode", polylineProperties.interp_color_mode);
        else
            if ~isempty(polylineProperties.interp_color_vector) then
                h.interp_color_vector = polylineProperties.interp_color_vector;
            end
            h.interp_color_mode = polylineProperties.interp_color_mode;
        end
        fields(fields=="interp_color_vector") = [];
        fields(fields=="interp_color_mode") = [];

        // Get mark_mode to be sure to set it after mark_style
        mark_mode = polylineProperties.mark_mode;
        fields(fields=="mark_mode") = [];

        global %POLYLINE
        %POLYLINE = h

        for i = 1:size(fields, "*")
            if fields(i) == "mark_style" then
                set(h, "mark_style", polylineProperties.mark_style);
                set(h, "mark_mode", mark_mode);
            elseif fields(i) == "children" then
                createMatrixHandle(polylineProperties(fields(i)));
            elseif fields(i) == "datatips" then
                createMatrixHandle(polylineProperties(fields(i)));
            else
                h(fields(i)) = polylineProperties(fields(i));
            end
        end

        clearglobal %POLYLINE

    endfunction

    //
    // PLOT3D
    //
    function h = createPlot3d(plot3dProperties)
        h = createSurface(plot3dProperties);
    endfunction

    //
    // FAC3D
    //
    function h = createFac3d(fac3dProperties)
        h = createSurface(fac3dProperties);
    endfunction

    //
    // SURFACE
    //
    function h = createSurface(surfaceProperties)
        fields = fieldnames(surfaceProperties);
        fields(1) = [];
        // plot3d modify the axes properties
        //  - Save it
        // - Draw plot3d
        // - Restore it
        a=gca();
        rotation_angles = a.rotation_angles;
        axes_visible = a.axes_visible;
        box = a.box;
        margins = a.margins;
        x_label_visible = a.x_label.visible;
        y_label_visible = a.y_label.visible;
        z_label_visible = a.z_label.visible;
        x_label_text = a.x_label.text;
        y_label_text = a.y_label.text;
        z_label_text = a.z_label.text;
        axes_isoview = a.isoview;

        if (or(surfaceProperties.color_flag==[2 5]) & ~or(fields=="cdata_mapping")) | ..
            ((surfaceProperties.color_flag>=2) & or(fields=="cdata_mapping")) then
            plot3d1(surfaceProperties.data.x, surfaceProperties.data.y, list(surfaceProperties.data.z, surfaceProperties.data.color))
        else
            plot3d(surfaceProperties.data.x,surfaceProperties.data.y,surfaceProperties.data.z)
        end
        fields(fields=="data") = [];

        // Restore this properties after plot3d.
        a.rotation_angles = rotation_angles;
        a.axes_visible = axes_visible;
        a.box = box;
        a.margins = margins;
        a.x_label.visible = x_label_visible;
        a.y_label.visible = y_label_visible;
        a.z_label.visible = z_label_visible;
        a.x_label.text = x_label_text;
        a.y_label.text = y_label_text;
        a.z_label.text = z_label_text;
        a.isoview = axes_isoview;

        // Get mark_mode to be sure to set it after mark_style
        mark_mode = surfaceProperties.mark_mode;
        fields(fields=="mark_mode") = [];

        h=gce();

        if or(fields=="cdata_mapping") then // Fac3d specific
            if surfaceProperties.color_flag >= 2 then
                set(h, "cdata_mapping", surfaceProperties.cdata_mapping);
            end
            fields(fields=="cdata_mapping") = [];
        end

        if surfaceProperties.clip_state == "on" then
            set(h,"clip_box", surfaceProperties.clip_box);
        end
        set(h,"clip_state",surfaceProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            if fields(i) == "mark_style" then
                set(h, "mark_style", surfaceProperties.mark_style);
                set(h, "mark_mode", mark_mode);
            else
                h(fields(i)) = surfaceProperties(fields(i));
            end
        end
    endfunction

    //
    // COMPOUND
    //
    function h = createCompound(compoundProperties)
        fields = fieldnames(compoundProperties);
        fields(1) = [];

        h = glue(createMatrixHandle(compoundProperties.children));
        fields(fields=="children") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), compoundProperties(fields(i)));
        end
    endfunction

    //
    // RECTANGLE
    //
    function h = createRectangle(rectangleProperties)
        fields = fieldnames(rectangleProperties);
        fields(1) = [];

        xrect(0,1,1,1); // create the rectangle with dummy values
        h = gce();

        if rectangleProperties.clip_state == "on" then
            set(h,"clip_box", rectangleProperties.clip_box);
        end
        set(h,"clip_state",rectangleProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        // Get mark_mode to be sure to set it after mark_style
        mark_mode = rectangleProperties.mark_mode;
        fields(fields=="mark_mode") = [];

        for i = 1:size(fields, "*")
            if fields(i) == "mark_style" then
                set(h, "mark_style", rectangleProperties.mark_style);
                set(h, "mark_mode", mark_mode);
            else
                h(fields(i)) = rectangleProperties(fields(i));
            end
        end
    endfunction

    //
    // ARC
    //
    function h = createArc(arcProperties)
        fields = fieldnames(arcProperties);
        fields(1) = [];

        xarc(0,1,1,1,0,360); // create the arc with dummy values
        h = gce();

        if arcProperties.clip_state == "on" then
            set(h,"clip_box", arcProperties.clip_box);
        end
        set(h,"clip_state",arcProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), arcProperties(fields(i)));
        end
    endfunction

    //
    // CHAMP
    //
    function h = createChamp(champProperties)
        fields = fieldnames(champProperties);
        fields(1) = [];

        champ(champProperties.data.x, champProperties.data.y, champProperties.data.fx, champProperties.data.fy);
        fields(fields=="data") = [];

        h=gce();

        if champProperties.clip_state == "on" then
            set(h,"clip_box", champProperties.clip_box);
        end
        set(h,"clip_state",champProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), champProperties(fields(i)));
        end
    endfunction

    //
    // SEG
    //
    function h = createSegs(segsProperties)
        fields = fieldnames(segsProperties);
        fields(1) = [];

        xsegs(segsProperties.data(:,1), segsProperties.data(:,2))

        h=gce()

        if segsProperties.clip_state == "on" then
            set(h,"clip_box", segsProperties.clip_box);
        end
        set(h,"clip_state",segsProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        // Get mark_mode to be sure to set it after mark_style
        mark_mode = segsProperties.mark_mode;
        fields(fields=="mark_mode") = [];

        for i = 1:size(fields, "*")
            if fields(i) == "mark_style" then
                set(h, "mark_style", segsProperties.mark_style);
                set(h, "mark_mode", mark_mode);
            else
                h(fields(i)) = segsProperties(fields(i));
            end
        end
    endfunction

    //
    // GRAYPLOT
    //
    function h = createGrayplot(grayplotProperties)
        fields = fieldnames(grayplotProperties);
        fields(1) = [];

        grayplot(grayplotProperties.data.x, grayplotProperties.data.y, grayplotProperties.data.z);
        fields(fields=="data") = [];

        h = gce();

        if grayplotProperties.clip_state=="on" then
            set(h, "clip_box", grayplotProperties.clip_box)
        end
        set(h, "clip_state", grayplotProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), grayplotProperties(fields(i)));
        end
    endfunction

    //
    // MATPLOT
    //
    function h = createMatplot(matplotProperties)
        fields = fieldnames(matplotProperties);
        fields(1) = [];

        Matplot(matplotProperties.data);
        fields(fields=="data") = [];

        h = gce();

        if matplotProperties.clip_state=="on" then
            set(h, "clip_box", matplotProperties.clip_box)
        end
        set(h, "clip_state", matplotProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), matplotProperties(fields(i)));
        end
    endfunction

    //
    // FEC
    //
    function h = createFec(fecProperties)
        fields = fieldnames(fecProperties);
        fields(1) = [];

        fec(fecProperties.data(:,1), fecProperties.data(:,2), fecProperties.triangles, fecProperties.data(:,3));
        fields(fields=="data") = [];
        fields(fields=="triangles") = [];

        h = unglue(gce());

        if fecProperties.clip_state=="on" then
            set(h, "clip_box", fecProperties.clip_box)
        end
        set(h, "clip_state", fecProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), fecProperties(fields(i)));
        end
    endfunction

    //
    // LEGEND
    //
    function h = createLegend(legendProperties)
        global %LEG
        %LEG = legendProperties;
        h = [];
    endfunction

    //
    // TEXT
    //
    function h = createText(textProperties)
        fields = fieldnames(textProperties);
        fields(1) = [];

        if textProperties.text_box_mode == "off" then
            xstring(textProperties.data(1), textProperties.data(2), textProperties.text)
        else
            xstringb(textProperties.data(1), textProperties.data(2), textProperties.text, textProperties.text_box(1), textProperties.text_box(2))
        end

        h = gce();

        if textProperties.clip_state=="on" then
            set(h, "clip_box", textProperties.clip_box)
        end
        set(h, "clip_state", textProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), textProperties(fields(i)));
        end
    endfunction

    //
    // DATATIP
    //
    function h = createDatatip(datatipProperties)

        fields = fieldnames(datatipProperties);
        fields(1) = [];

        tip_data = datatipProperties("data");
        h = datatipCreate(%POLYLINE, tip_data);

        for i = 1:size(fields, "*")
            if fields(i) == "data" then
                continue;
            end

            set(h, fields(i), datatipProperties(fields(i)));
        end
    endfunction

    //
    // AXIS
    //
    function h = createAxis(axisProperties)
        fields = fieldnames(axisProperties);
        fields(1) = [];

        if axisProperties.tics_direction == "bottom" then
            axisdir="d";
        elseif axisProperties.tics_direction == "top" then
            axisdir="u";
        elseif axisProperties.tics_direction == "left" then
            axisdir="l";
        elseif axisProperties.tics_direction == "right" then
            axisdir="r";
        elseif size(axisProperties.xtics_coord, "*") > 1 then
            axisdir="u";
        else
            axisdir="l";
        end
        fields(fields=="tics_direction") = [];

        drawaxis(x=axisProperties.xtics_coord,y=axisProperties.ytics_coord,dir=axisdir);
        fields(fields=="xtics_coord") = [];
        fields(fields=="ytics_coord") = [];

        h=gce()

        if axisProperties.clip_state=="on" then
            set(h, "clip_box", axisProperties.clip_box)
        end
        set(h, "clip_state", axisProperties.clip_state);
        fields(fields=="clip_box") = [];
        fields(fields=="clip_state") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), axisProperties(fields(i)));
        end
    endfunction

    //
    // uimenu
    //
    function h = createuimenu(uimenuProperties)
        fields = fieldnames(uimenuProperties);
        fields(1) = [];

        h = uimenu();

        for i = 1:size(fields, "*")
            if fields(i) == "children" then
                children = createMatrixHandle(uimenuProperties(fields(i)));
                for k=1:size(children, "*")
                    set(children(k), "parent", h);
                end
            else
                set(h, fields(i), uimenuProperties(fields(i)));
            end
        end
    endfunction

    //
    // UICONTEXTMENU
    //
    function h = createuicontextmenu(uicontextmenuProperties)
        fields = fieldnames(uicontextmenuProperties);
        fields(1) = [];

        h = uicontextmenu();

        for i = 1:size(fields, "*")
            if fields(i) == "children" then
                children = createMatrixHandle(uicontextmenuProperties(fields(i)));
                for k=1:size(children, "*")
                    set(children(k), "parent", h);
                end
            else
                set(h, fields(i), uicontextmenuProperties(fields(i)));
            end
        end
    endfunction

    //
    // uicontrol
    //
    function h = createuicontrol(uicontrolProperties)
        fields = fieldnames(uicontrolProperties);
        fields(1) = [];

        if or(fields=="scrollable") then
            // Properties added in Scilab 5.5.0
            //  - scrollable must be set at creation (for frames)
            //  - constraints & margins must be set before parent
            h = uicontrol("style", uicontrolProperties.style, ...
            "scrollable", uicontrolProperties.scrollable, ...
            "constraints", uicontrolProperties.constraints, ...
            "margins", uicontrolProperties.margins);
            fields(fields=="scrollable") = [];
            fields(fields=="constraints") = [];
            fields(fields=="margins") = [];
            h.layout_options = uicontrolProperties.layout_options;
            fields(fields=="layout_options") = [];
            h.layout = uicontrolProperties.layout;
            fields(fields=="layout") = [];
        else
            h = uicontrol("style", uicontrolProperties.style);
        end
        fields(fields=="style") = [];

        for i = 1:size(fields, "*")
            if fields(i) == "children" then
                children = createMatrixHandle(uicontrolProperties(fields(i)));
                for k=1:size(children, "*")
                    set(children(k), "parent", h);
                end
            else
                set(h, fields(i), uicontrolProperties(fields(i)));
            end
        end
    endfunction

    //
    // LIGHT
    //
    function h = createLight(lightProperties)
        fields = fieldnames(lightProperties);
        fields(1) = [];

        h = light();
        fields(fields=="children") = [];

        for i = 1:size(fields, "*")
            set(h, fields(i), lightProperties(fields(i)));
        end
    endfunction

    // Utility function for legends, copy/paste from %h_load
    function links=getlinksfrompath(ax,paths)
        //  ax is a  handle on an axes entity
        //  paths a list or row vector which gives the set of paths relative to
        //  the axes
        links=[];
        ok=%t;
        for p=paths
            e=ax;
            p(1)=p(1)-1// the caption does not exists yet
            for kp=1:size(p,"*"),
                if or(e.type==["Axes","Compound"])&p(kp)<=size(e.children,"*") then
                    e=e.children(p(kp)),
                else
                    ok=%f
                    break
                end
            end
            if ~ok then
                break
            end
            links=[links,e]
        end
        if ~ok then
            links=[];
        end
    endfunction

    function macro = createMacro(macroStr, macroName)
        macroSt = macroStr(3);
        if macroStr(2) == %t then
            flag = "c";
        else
            flag = "n";
        end
        header = strsubst(macroSt(1), "function ", "");
        body = macroSt(2:$-1);
        if body == [] then
            body = "";
        end
        deff(header, body, flag);
        execstr("macro = " + macroName);
    endfunction

    varargout = list();
    for i = 1:size(%__varnameList__, "*")
        varargout(i) = %__convertVariable__(evstr(%__varnameList__(i)), %__varnameList__(i));
        //printf("add %s: type %s become %s\n", %__varnameList__(i), typeof(evstr(%__varnameList__(i))), typeof(varargout(i)));
    end
endfunction
