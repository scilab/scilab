// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ged_move_entity()
    // Internal private function called by ged(6,fig)
    //  - Follows the mouse
    //  - When it is left or right clicked, tries to detect a graphical component
    //    in its vicinity.
    //  - If a component is detected,
    //    - then moves it following the mouse movements
    //    - sets its new position when the mouse is left or right clicked
    //  -... until the center button is clicked or 15 s are elapsed without
    //    moving the mouse on the figure.
    //
    //  BUG: When the mouse goes out of the figure where the object has been
    //       picked, on another figure, the object may reappear on the current
    //       figure, far from where the mouse stands.
    //       See http://bugzilla.scilab.org/13913

    f0 = gcf();
    f = f0;
    if f.default_axes=="off" then
        // xclick() and xgetmouse() are disabled in such figures
        return
    end
    ax0 = gca();                // Save it to restore it before leaving
    btn = 0;
    maxRestTime = 15; // s
    warning(_("Mouse moving picker ON => Console LOCKED"))
    mprintf(_("On a figure: Click left to get and set. Click middle to QUIT...\n"))
    while ~or(btn==[1 4]);  // click or press middle button => Quit
        tic()
        [btn, xc, yc, nFig, cb] = xclick(); // [xc, yc] are in data scales
        if nFig ~= f.figure_id
            f = scf(nFig);
        end
        if toc()>maxRestTime
            break
        end
        pos = [xc,yc];
        [xc,yc] = xchange(xc,yc,"f2i");  // [xc, yc] => in pixels. Origin on the top-left
        [r,ax] = ged_getobject([xc,yc]); // <-- Detects the clicked object
        cur_ax = gca()
        if ax.type=="Axes"
            sca(ax);
            Xlog = part(ax.log_flags,1)=="l";
            Ylog = part(ax.log_flags,2)=="l";
            [xc,yc] = xchange(xc,yc,"i2f");
        else    // uicontrols: coordinates in pixels
            Xlog = %f;
            Ylog = %f;
            // [xc, yc] remain in pixels coordinates
        end
        pos = [xc,yc]
        if r~=[] & type(r)==9
            rep(3) = -1;
            //disp(r.type+" clicked")
            select r.type
            case "Rectangle" then
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    [nx, ny] = ged_newObjectPosition(r.data, rep, pos);
                    // The sizes of the rectangle are not changed.
                    // So in log scale(s) they are rendered in an elastic way.
                    r.data(1:2) = [nx ny];
                    pos = rep(1:2);
                end
            case "Segs" then //Segment
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    r.data = r.data + ones(r.data(:,1))*(rep(1:2)-pos);
                    pos = rep(1:2);
                end
            case "Polyline" then //Polyline
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    r.data(:,1:2) = r.data(:,1:2) + ones(r.data(:,1))*(rep(1:2)-pos);
                    pos = rep(1:2);
                end
            case "Arc" then //Circle
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    [nx, ny] = ged_newObjectPosition(r.data, rep, pos);
                    // The sizes of the enclosing rectangle of the arc are not changed.
                    // So in log scale(s) they are rendered in an elastic way.
                    r.data(1:2) = [nx ny];
                    pos = rep(1:2);
                end

            case "Text" then
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    [nx, ny] = ged_newObjectPosition(r.data, rep, pos);
                    r.data = [nx ny];
                    pos = rep(1:2);
                end
            case "Label" then
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    [nx, ny] = ged_newObjectPosition(r.position, rep, pos);
                    r.auto_position = "off";
                    r.position = [nx ny];
                    pos = rep(1:2);
                end
            case "Axes" then
                // The axes coordinates are in normalized units in the figure
                cf = ged_coordinates_in_figure(ax, xc, yc);
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    ncf =  ged_coordinates_in_figure(ax, rep(1), rep(2));
                    ax.axes_bounds(1:2) = ax.axes_bounds(1:2) + (ncf - cf);
                    cf = ncf;
                end
            case "Datatip" then
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t])
                    p = r.detached_position
                    [nx, ny] = ged_newObjectPosition(p, rep, pos)
                    r.detached_position = [nx ny p(3)]
                    pos = rep(1:2)
                end
            case "Legend" then
                // Legend block generated with legend()
                // (legends() (with "s") sets an axes instead: move axes
                //  not yet supported)
                //
                // * reversed and log axes supported
                // * prone to slight error if axes.tight_limits=="off"
                //   (no way to take it into account)
                r.legend_location = "by_coordinates";
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    nx = rep(1); // New x position in data unit
                    ny = rep(2); // New y position in data unit
                    // Conversion in normalized axes units:
                    db = ax.data_bounds;
                    xlog = part(ax.log_flags,1)=="l";
                    if xlog
                        db(1:2) = log10(db(1:2));
                        nx = log10(nx);
                    end
                    if ax.axes_reverse(1)=="off"
                        nxa = (nx - db(1)) / (db(2)-db(1)) * ..
                              (1-sum(ax.margins(1:2))) + ax.margins(1);
                    else
                        nxa = (db(2) - nx) / (db(2)-db(1)) * ..
                              (1-sum(ax.margins(1:2))) + ax.margins(1);
                    end
                    ylog = part(ax.log_flags,2)=="l";
                    if ylog
                        db(3:4) = log10(db(3:4));
                        ny = log10(ny);
                    end
                    if ax.axes_reverse(2)=="off"
                        nya = (db(4) - ny) / (db(4)-db(3))  * ..
                              (1-sum(ax.margins(3:4))) + ax.margins(3);
                    else
                        nya = (ny - db(3)) / (db(4)-db(3))  * ..
                              (1-sum(ax.margins(3:4))) + ax.margins(3);
                    end
                    r.position = [nxa nya];
                    pos = rep(1:2);
                end

            case "uicontrol"
                Fx = 1; Fy = 1;
                if r.units=="normalized"
                    Fx = f.axes_size(1);
                    Fy = f.axes_size(2);
                end
                while rep(3)==-1 do
                    rep = xgetmouse([%t %t]);
                    [x, y] = xchange(rep(1),rep(2), "f2i");
                    r.position(1:2) = r.position(1:2) + [(x-pos(1))/Fx, (pos(2)-y)/Fy];
                    pos = [x y];
                end
            end
            sca(cur_ax);
        end
    end
    // Restoring initial current figure and axes:
    scf(f0);
    mprintf(_("\n--> // Moving picker OFF => Back to the console.\n"))
endfunction

function [nx, ny] = ged_newObjectPosition(objPos, pt, prevpt)
    if ~Xlog
        nx = objPos(1) + (pt(1)-prevpt(1))
    else
        nx = objPos(1)*(pt(1)/prevpt(1))
    end
    if ~Ylog
        ny = objPos(2) + (pt(2)-prevpt(2))
    else
        ny = objPos(2)*(pt(2)/prevpt(2))
    end
endfunction

function xy_in_figure = ged_coordinates_in_figure(idA, xdata,ydata)
    // reversed and log axes supported
    ab = idA.axes_bounds
    db = idA.data_bounds
    if part(idA.log_flags,1)=="l" then
        db(1:2) = log10(db(1:2))
        xdata = log10(xdata)
    end
    dx = xdata - db(1)
    if idA.axes_reverse(1)=="on" then
        dx = db(2) - xdata
    end
    if part(idA.log_flags,2)=="l" then
        db(3:4) = log10(db(3:4))
        ydata = log10(ydata)
    end
    dy = db(4) - ydata
    if idA.axes_reverse(2)=="on" then
        dy = ydata - db(3)
    end
    xf = (dx/(db(2)-db(1))*(1-sum(idA.margins(1:2))) + idA.margins(1))*ab(3) + ab(1)
    yf = (dy/(db(4)-db(3))*(1-sum(idA.margins(3:4))) + idA.margins(3))*ab(4) + ab(2)
    xy_in_figure = [xf yf]
endfunction
