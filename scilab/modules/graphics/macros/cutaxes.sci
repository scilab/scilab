// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Axes, hcurves] = cutaxes(varargin)
    // [Axes, hcurves] = cutaxes(x0, y0, cutaxis, intervals, ..)
    // Axes = cutaxes(ax, cutaxis, intervals, ..)

    // INITIALIZATION
    // --------------
    fname = "cutaxes"

    // ARGUMENTS ANALYSIS
    // ------------------
    if argn(2)<3 then
        if exists("uman"), uman("cutaxes","u"); end
        msg = _("%s: Wrong number of input arguments: At least %d expected.\n");
        error(msprintf(msg, fname, 3))
    end
    // Input content: Axes or explicit curves
    x0 = varargin(1);
    if ~or(type(x0)==[1 9]) then
        if exists("uman"), uman("cutaxes","u"); end
        msg = _("%s: Argument #%d: Axes handle or vector of decimal numbers expected.\n");
        error(msprintf(msg, fname, 1));
    end

    fromAxes = type(x0)==9;
    if fromAxes
        if x0.type~="Axes" then
            if exists("uman"), uman("cutaxes","u"); end
            msg = _("%s: Argument #%d: Axes handle expected.\n");
            error(msprintf(msg, fname, 1));
        end
        ia = 2;  // index of next argument
    else
        x0 = x0(:);
        varargin(1) = null();
        y0 = varargin(1);
        if min(size(y0))==1 then
            y0 = y0(:)
        end
        nc = size(y0,2)     // number of curves
        ia = 3;
    end

    varargin(1) = null();
    if modulo(length(varargin),2)~=0
        if exists("uman"), uman("cutaxes","u"); end
        msg = _("%s: Wrong number of input arguments: incomplete {''name'', value} pair.\n");
        error(msprintf(msg, fname))
    end

    // Chosen axis:
    cutaxis = varargin(1)(1);
    varargin(1) = null();
    if type(cutaxis)~=10 | and(convstr(cutaxis)~=["x" "y"]) then
        if exists("uman"), uman("cutaxes","u"); end
        msg = _("%s: Argument #%d: ''x'' or ''y'' expected.\n");
        error(msprintf(msg, fname, ia+1))
    end
    intervals = varargin(1);
    varargin(1) = null();

    // Options:
    for i = 1:2:length(varargin)
        v = varargin(i)
        if typeof(v)~="string" | size(v,"*")>1 | ~or(v==["ticksmode" "widths"])
            if exists("uman"), uman("cutaxes","u"); end
            msg = _("%s: Argument #%d: one word among {%s} expected.\n");
            error(msprintf(msg,fname, 3+2*i, """ticksmode"",""widths"""));
        else
            execstr(v + "=varargin(i+1)")
        end
    end
    if ~isdef("ticksmode", "l") then
        ticksmode = ""
    end
    if ~isdef("widths", "l") then
        widths = "equal"
    else
        if type(widths)==1
            if length(widths)~=size(intervals,1)
                if exists("uman"), uman("cutaxes","u"); end
                msg = _("%s: widths vector: %d components expected.\n");
                error(msprintf(msg, fname, size(intervals,1)));
            end
            widths(1) = -abs(widths(1))  // First section has never heading interspace
        end
    end

    // Filtering data for min and max of x and y
    // -----------------------------------------
    if ~fromAxes then
        ksel = []
        if cutaxis=="x" then
            for intv = intervals'
                ksel = [ksel find(x0>=min(intv) & x0<=max(intv))]
            end
            // ksel = unique([ksel, ksel-1, ksel+1]) // keeping segments crossing axis bounds
            // if ksel(1)==0, ksel(1) = [], end
            // if ksel($)>length(x0), ksel($) = [], end
            ksel = unique(ksel)
            x = x0(ksel)
            y = y0(ksel,:)

        else    // "y" breaks
            for intv = intervals'
                ksel = [ksel find(y0>=min(intv) & y0<=max(intv))]
            end
            ksel = unique(ksel)
            xsel = ind2sub(size(y0), ksel)
            x = x0(xsel)
            y = y0(ksel)
        end
        // min & max
        xmin = min(x)
        xmax = max(x)
        ymin = min(y)
        ymax = max(y)
    else
        db = x0.data_bounds
        [xmin, xmax, ymin, ymax] = (db(1),db(2),db(3),db(4));
    end

    fig = gcf();
    rendering_old = fig.immediate_drawing;
    fig.immediate_drawing = "off";

    // Plotting the common axes
    // ------------------------
    ax0 = gca()
    del_x0 = %f

    if fromAxes & x0==gca() then  // replace
        s = ax0.axes_bounds;
        ax0 = newaxes();
        ax0.axes_bounds = s;
        ax0.title.text = x0.title.text;
        ax0.title.text = x0.title.text;
        del_x0 = %t
    end
    ax0.visible = "off"

    // Transfering titles of the initial axes
    if fromAxes then
        prop = ["text" "font_foreground" "font_style" "font_angle" "user_data" ..
                "fractional_font" "font_size" "auto_position" "position" ..
                "fill_mode" "foreground" "background" "auto_rotation"]
        for p = prop
            execstr("ax0.title."+p+"=x0.title."+p);
            execstr("ax0.x_label."+p+"=x0.x_label."+p);
            execstr("ax0.y_label."+p+"=x0.y_label."+p);
        end
    end

    plot2d([],[],0, "011"," ", [xmin ymin xmax ymax])

    if cutaxis=="x"
        ax0.axes_visible(1) = "off"
        ax0.grid = [-1 color("grey70")] // y-grid only
        if ax0.y_label.text==""
            ylabel(".")   // to enlarge the margin to anticipate a true legend
        end
        margins = ax0.margins;
    else
        ax0.axes_visible(2) = "off"
        ax0.grid = [color("grey70") -1] // x-grid only
        if ax0.x_label.text==""
            xlabel(".")   // to enlarge the margin to anticipate a true legend
        end
        margins = ax0.margins;
    end
    ax0.box = "off"
    ax0.filled = "off"
    ax0.grid_style = [8 8]

    // Setting axes_bounds of subaxes
    // ------------------------------
    ax0b = ax0.axes_bounds  // all must be plotted in this area
        // The relative cumulated separating space rcss is such that 0.05 <= rcss <= 0.1
    na = size(intervals,1)
    if typeof(widths)=="string" & part(widths,1)=="-" then
        rcss = 0;
        rss = 0;
    else
        if type(widths)==1
            nas = sum(widths(2:$)<0)
        else
            nas = na-1
        end
        if na<11
            rss = 0.01
            rcss = nas*rss
        else
            rcss = 0.1
            rss = rcss / nas
        end
    end
        //
    if or(widths==["isoscaled" "-isoscaled"]) then
        Dxy = abs(intervals(:,2)-intervals(:,1))  // absolute widths of selected intervals
    elseif type(widths)==1  // values
        Dxy = abs(widths(:))
    else                    // equal
        Dxy = ones(na,1)
    end
    Dxyr = Dxy/sum(Dxy) // *(1-rcss) A VOIR  // relative widths ...
    if cutaxis=="x" then
        wt = ax0b(3)*(1-sum(margins(1:2)))    // Total width
        x_left = ax0b(1) + ax0b(3) * margins(1) // initial left margin
        y_top = ax0b(2)  // Same vertical position for all subaxes
        h = ax0b(4)      // Same height
    else
        x_left = ax0b(1) // same x position for all subaxes
        w = ax0b(3)      // Same width for all subaxes
        ht = ax0b(4)*(1-sum(margins(3:4)))    // Total height
        // initial top margin:
        y_top  = ax0b(2) + ax0b(4) * (1 - margins(4)) + rss*ht
    end

    hcurves = list([],[])
    Axes = []
    iba = 1     // index of broken axis: "x" => 1,  "y" => 2
    if cutaxis ~= "x" then
        iba = 2
    end
    for ia = 1:na
        interv = intervals(ia,:);
        if fromAxes then
            ax_i = copy(x0)
            xtitle("","","","")
            ax_i.isoview = "off";
        else
            ax_i = newaxes()
        end
        ax_i.visible = "off"
        ax_i.margins(2*iba+(-1:0)) = 0
        ax_i.tight_limits = "on"
        ax_i.grid_style = [8 8];
        ax_i.grid(iba) = color("grey70")
        ax_i.filled = "off"
        ax_i.clip_state = "off"     // to not clip datatips set on edges
        if cutaxis=="x"
            ax_i.axes_bounds = [x_left, y_top, Dxyr(ia)*wt, h]

            if (type(widths)==10 & part(widths,1)=="-") | ..
                (type(widths)==1 & ia<na & widths(ia+1)<0) then
                tmp = 0
            else
                tmp = rss
            end
            x_left = x_left + (Dxyr(ia)+tmp)*wt

            if ~fromAxes then
                k = find(x0>=min(interv) & x0<=max(interv))
                plot(x0(k), y0(k,:))
            end
            ax_i.data_bounds = [min(interv) max(interv) ; ymin ymax]'
            if ticksmode=="alt" & modulo(ia,2)==0
                ax_i.x_location = "top"
            elseif ticksmode=="shift" & modulo(ia,2)==0
                dyr = 0.025
                ax_i.axes_bounds(4) = h*(1+dyr)
                ax_i.data_bounds(:,2) = [ymin-(ymax-ymin)*dyr; ymax]
            end
            if interv(1)>interv(2) then
                ax_i.axes_reverse(1) = "on";
            end
        else
            if (type(widths)==10 & part(widths,1)=="-") | ..
                (type(widths)==1 & ia<=na & widths(ia)<0) then
                tmp = 0
            else
                tmp = rss
            end
            y_top = y_top - (Dxyr(ia)+tmp)*ht
            ax_i.axes_bounds = [x_left, y_top, w, Dxyr(ia)*ht]

            if ~fromAxes then
                plot(x0,y0)
            end
            ax_i.data_bounds = [xmin xmax ; min(interv) max(interv)]'
            if ticksmode=="alt" & modulo(ia,2)==0
                ax_i.y_location = "right"
            elseif ticksmode=="shift" & modulo(ia,2)==0
                dxr = 0.025
                ax_i.axes_bounds(3) = w*(1+dxr)
                ax_i.axes_bounds(1) = ax_i.axes_bounds(1) - w*dxr
                ax_i.data_bounds(:,1) = [xmin-(xmax-xmin)*dxr; xmax]
            end
            if interv(1)>interv(2) then
                ax_i.axes_reverse(2) = "on";
            end
        end
        if ~fromAxes
            tmp = gce()
            for ic = 1:nc
                hcurves(nc-ic+1) = [hcurves(nc-ic+1) tmp.children(ic)]
            end
        end
        ax_i.box = "off";
        ax_i.axes_visible = ["off" "off" "off"]
        ax_i.axes_visible(iba) = "on"
        Axes = [Axes ax_i]
    end
    if del_x0 then
        delete(x0)
    end

    Axes = [Axes ax0]
    Axes.visible = "on";
    Axes.children.visible = "on";
    sca(ax0);

    fig.immediate_drawing = rendering_old;
endfunction
