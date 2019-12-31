// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function sgrid(varargin)
    //    sgrid()
    //    sgrid(zeta, wn)
    //    sgrid(.., colors)
    //    sgrid(.., "new") // at any position

    colors = [color("cyan") color("skyblue1")];  // [circles, rays]
    defaultbounds = [-1.6,-1.6;0,1.6];
    defaultZeta = [0 0.17 0.34 0.5 0.64 0.77 0.87 0.94 0.985 1];

    // CHECKING INPUT ARGUMENTS
    // ========================
    if argn(2) > 4 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "sgrid", 0, 4))
    end

    // New?
    // ----
    new = 0;
    i = 1
    while length(varargin)>0 & i <= length(varargin)
        a = varargin(i)
        if type(a)==10 & a(1)=="new"
            new = i
            varargin(i) = null()
            break
        else
            i = i+1
        end
    end
    rhs = length(varargin)

    // zeta, wn
    // --------
    defaultwn = %f;
    if or(rhs==[0 1]) then
        zeta = defaultZeta
        defaultwn = %t
        j = 1;
    else
        zeta =  varargin(1)
        if type(zeta)<>1 | ~isreal(zeta) then
            msg = _("%s: Wrong type for input argument #%d : real floating point array expected\n")
            j = 1; if new & new<2, j = 2, end
            error(msprintf(msg, "sgrid", j));
        end
        zeta = zeta(zeta>=0 & zeta<=1)
        // if zeta==[], zeta = defaultZeta, end

        wn = varargin(2)
        if type(wn)<>1 | ~isreal(wn) then
            msg = _("%s: Wrong type for input argument #%d : real floating point array expected\n")
            j = 2; if new & new<3, j = 3, end
            error(msprintf(msg, "sgrid", j));
        end
        wn = abs(wn)
        // if wn==[], defaultwn = %t, end
        j = 3;
    end

    // colors
    // ------
    if length(varargin)>=j
        colors = iscolor(varargin(j), "1#a")
        if new & new<=j, j = j+1, end
        if size(colors,1)>2
            msg = _("%s: Argument #%d: Scalar or vector with %d components expected.\n")
            error(msprintf(msg, "sgrid", j, 2));
        end
        if or(isnan(colors))
            msg = _("%s: Argument #%d: Wrong color specification.\n")
            error(msprintf(msg, "sgrid", j));
        end
        if size(colors,2)==3   // RGB
            if size(colors,1)==1 then
                colors = [1;1]*colors
            end
            colors = addcolor(colors)
        end                  // else: Indices => OK
    end

    // Graphics initializations
    // ------------------------
    fig = gcf();
    immediate_drawing = fig.immediate_drawing;
    fig.immediate_drawing = "off";
    axes = gca();
    if new & axes.children<>[] then
        delete(axes.children)
    end
    nc = size(axes.children,"*")
    if nc==0 then
        if defaultwn
            data_bounds = defaultbounds;
        else
            data_bounds = [-1 0 -1 1]*max(wn)
        end
        axes.data_bounds = data_bounds;
        axes.axes_visible = "on";
        ax.box = "on";
    else
        data_bounds = [min(defaultbounds(1,:), axes.data_bounds(1,:));
                       max(defaultbounds(2,:), axes.data_bounds(2,:))];
    end

    if axes.tight_limits(1)=="off" & axes.auto_ticks(1)=="on" then
        data_bounds([1 2]) = [min(data_bounds(1), min(axes.x_ticks.locations)) ..
                              max(data_bounds(2), max(axes.x_ticks.locations))]
    end
    if axes.tight_limits(2)=="off" & axes.auto_ticks(2)=="on" then
        data_bounds([3 4]) = [min(data_bounds(3), min(axes.y_ticks.locations)) ..
                              max(data_bounds(4), max(axes.y_ticks.locations))]
    end

    db = data_bounds;
    [xmin, xmax, ymin, ymax] = (db(1), db(2), db(3),db(4));

    //iso natural frequency curves (circles centered on (0,0))
    // -------------------------------------------------------
    if defaultwn then
        rmax = sqrt(min(xmin^2+ymin^2,xmin^2+ymax^2))
        rmax = floor(10*rmax)/10;
        [xi, xa, tmp, np] = xgraduate(0,rmax);
        wn = linspace(xi,xa,np+1)
        wn($) = [];
    end

    // Setting the angle of radial labels
    axes_size = gcf().axes_size .* axes.axes_bounds(3:4);
    m = axes.margins;
    h = 1-sum(axes.margins(3:4));
    w = 1-sum(axes.margins(1:2));
    //axes_size = axes_size .* [w h];
    f = axes_size(1) / axes_size(2);
    f2 = abs((xmin)/(ymax-ymin));
    if abs(ymin) >= abs(ymax) then
        diag_angle = atan(ymin, xmin)
    else
        diag_angle = atan(ymax, xmin)
    end

    // Typical text size
    rect = xstringl(1,1,"0.3")
    w = rect(3);
    h = rect(4);   // prone to http://bugzilla.scilab.org/13661 , so:
    h = w*0.5/(xmax-xmin)*(ymax-ymin);

    // Drawing circles and radial labels
    wn = wn(wn>=0)
    t = linspace(%pi/2,1.5*%pi,200)
    chart_handles = []
    for i = 1:size(wn,"*")
        // Circle
        xpoly(wn(i)*cos(t), wn(i)*sin(t))
        ec = gce();
        ec.display_function = "formatSgridFreqTip";
        ec.display_function_data = wn(i);
        ec.foreground = colors(1),
        ec.line_style = 7;
        ec.clip_state = "clipgrf";

        // Labels
        lab = msprintf("%.2g",wn(i))
        r = xstringl(0, wn(i), lab)
        xstring(w/3, wn(i)-h*1.5, lab)
        xstring(w/3, -wn(i)-h*0.5, lab)
        xstring(wn(i)*cos(diag_angle), wn(i)*sin(diag_angle), lab)
        lab = gce().parent.children(1:3);
        if i==1, lab(2:3).visible = "off"; end // 0 only once
        lab.font_foreground = colors(1),
        lab(1).clip_state = "clipgrf";
        if -wn(i)<ymin, lab(2).clip_state = "clipgrf"; end
        if  wn(i)>ymax, lab(3).clip_state = "clipgrf"; end
        lab(1).text_box = [0 0];
        lab(1).text_box_mode = "centered";
        e = glue([lab ; ec])

        chart_handles = [e chart_handles]
    end

    // iso damping factor curves
    // -------------------------
    angl = acos(zeta)
    for i = 1:size(zeta,"*")
        lab = string(zeta(i))
        rect = xstringl(0,0,lab)

        a = angl(i)
        ta = tan(a);
        if a==0
            xpoly([0 xmin],[0 0])
            xstring(xmin, -h/6, lab)
            xpoly([0 xmin],[0 0])
            xstring(xmin, -h/6, lab)
        else
            // upper
            y = min(-xmin*ta, ymax)
            xpoly([0 -y/ta],[0 y])
            if y < ymax-h/2
                xstring(xmin, y-h/7, lab)
            else
                xstring((-y+h)/ta, ymax-h, lab) // , a*180/%pi
                set(gce(), "text_box", [0 0], "text_box_mode", "centered");
            end
            // lower
            y = max(xmin*ta, ymin)
            xpoly([0 y/ta],[0 y])
            if y > ymin
                xstring(xmin, y-h/6, lab)
            else
                xstring((y+h)/ta, ymin+h, lab) // , -a*180/%pi
                set(gce(), "text_box", [0 0], "text_box_mode", "centered");
            end
        end
        // rays
        ec = gce().parent.children([2 4]);
        ec.display_function = "formatSgridDampingTip";
        ec.display_function_data = zeta(i);
        ec.foreground = colors(2),
        ec.line_style = 7;
        ec.clip_state = "clipgrf";
        // labels
        et = gce().parent.children([1 3]);
        et.font_foreground = colors(2),
        et.clip_state = "clipgrf";
        // handles
        chart_handles = [glue([et(1) ec(1)]), glue([et(2) ec(2)]), chart_handles]

    end
    chart_handles = glue(chart_handles)

    //reorder axes children to make chart drawn before the black previously
    // drawn curves if any
    for k = 1:nc
        swap_handles(axes.children(k), axes.children(k+1))
    end

    axes.data_bounds = data_bounds;
    fig.immediate_drawing = immediate_drawing;
    show_window()
endfunction
