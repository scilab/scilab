// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013, 2016 - Samuel GOUGEON
// Copyright (C) 2016 - Clément DAVID - Scilab Enterprises
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function replot(varargin)
    // replot(rect, handl, tight_limits)
    // replots the graphic axes using rect as x and y (and z) bounds
    // rect = [xmin ymin xmax ymax] or [xmin ymin zmin xmax ymax zmax]
    // Added in Scilab 5.5 (2013):
    //  - idAxes|idFig option
    //  - %nan usage
    //  - extension to 3D
    // Added in Scilab 6.0 (2016):
    //  - Any handle (expected to have Axes children) such as Figures and
    //    frame uicontrols can now be specified.
    //  - buildEnclosingBounds() and management of %inf values
    //  - replot()
    //  - tight_limits option

    [lhs,rhs] = argn(0);
    fname = "replot"

    // CHECKING ARGUMENTS
    // ==================
    // Number of argins
    if rhs > 3 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 0, 3))
    end
    // Types and order
    //   If several argins with a same allowed type are provided,
    //   only the last one is considered
    for i = 1:size(varargin)
        a = varargin(i)
        if type(a)==0   // allows ,, syntaxes
            continue
        end
        msg = _("%s: Argument #%d is badly placed. Please check available syntaxes.\n")
        if a==[]
            continue
        elseif or(type(a)==[1 5 8])
            if isdef("handl", "l") || isdef("tight","l")
                error(msprintf(msg, fname, i))
            end
            rect = full(double(a))
            irect = i   // for possible forthcoming error message
        elseif type(a)==9
            if isdef("tight","l") // handles must be _before_ tight_limits
                error(msprintf(msg, fname, i))
            end
            handl = a
            ihandl = i
        elseif type(a)==10
            tight = a
            itight = i
        else
            msg = _("%s: Argument #%d: Wrong type. Please check available syntaxes.\n")
            error(msprintf(msg, fname, i))
        end
    end

    // Checking existence, formats and values. Assigning default values
    // Handles
    // -------
    if ~isdef("handl", "l")
        handl = gca()
    else
        Handl = []
        // We get Axes from handl() or from their direct children
        //  (if some are Figure, frame uicontrol, ..)
        for i = 1:length(handl)
            h = handl(i)
            if h.type=="Axes"
                Handl = [Handl ; h]
            else
                tmp = h.children
                Handl = [Handl ; tmp(tmp.type=="Axes")]
            end
        end
        handl = Handl
        if Handl==[]
            return
        end
    end

    // rect
    // ----
    if ~isdef("rect","l") then
        rect = %inf * ones(size(handl,1),6)
    end
    if or(length(rect)==[4 6]) then
        rect = rect(:)'     // Forces format in single row
    end
    // rect must have as many rows as targeted axes.
    // Otherwise, we trim it or replicate the only row or add %inf rows
    nr = size(rect,1)
    na = length(handl)  // number of axes
    if nr ~= na then
        if nr>na
            rect = rect(1:na,:)
        elseif nr==1    // the same rect is used for all Axes
            rect = ones(na,1)*rect(:)'
        else            // %inf are used for missing rows
            rect = [rect ; %inf*ones(na-nr, size(rect,2))]
        end
    end
    // rect must have provided 4 (2D) or 6 (3D) columns
    nc = size(rect,2)
    if ~or(nc==[4 6])
        msg = _("%s: Argument #%d: Matrix with 4 or 6 columns expected.\n")
        error(msprintf(msg, fname, irect))
    end
    // To get a standard 6 columns shape, we add zmin=%inf and zmax=%inf columns
    if nc==4
        rect = [rect(:,[1 2]) %inf*ones(na,1) rect(:,[3 4]) %inf*ones(na,1)]
    end

    // tight
    // -----
    if ~isdef("tight", "l") then
        tight = []
    else
        if size(tight,"*")>3
            tight = tight(1:3)  // We simply ignore extra values
        end
        tight = convstr(tight)  // => case-insensitive "on" "off" values
        if ~and(tight=="tight" | tight=="on" | tight=="off")
            msg = _("%s: Argument #%d: ""tight"" or ""on"" or ""off"" values expected.\n")
            error(msprintf(msg, fname, itight))
        elseif or(tight=="tight")
            tight(tight=="tight") = "on"
        end
        if size(tight,"*")==1   // If it is scalar => we replicate it 3 times
            tight = emptystr(1,3) + tight
        end
    end

    // PROCESSING
    // ==========
    // Loop over axes
    // --------------
    for i = 1:na
        bounds = matrix(rect(i,:)',-1,2)'
        // [ xmin  ymin  zmin
        //   xmax  ymax  zmax ]
        if handl(i).view=="2d"
            bounds(:,3) = []
        end
        k = ~isnan(bounds)  // Nan bounds are ignored
        // processing %inf values = set to data limits
        getTrueBounds = bounds==%inf
        if or(getTrueBounds)
            dataBounds = buildEnclosingBounds(handl(i))
            bounds(getTrueBounds) = dataBounds(getTrueBounds)
            handl(i).zoom_box = []
        end
        handl(i).data_bounds(k) = bounds(k)
        if tight~=[]
            handl(i).tight_limits = tight
        end
    end
endfunction

// ==========================================================================

function bounds = buildEnclosingBounds(e,bounds)
    // internal private function: utility for replot()
    select e.type
        case "Axes"
            nc = length(e.children)
            if nc==0 then
                bounds = e.data_bounds
                return
            end
            // We scan all components belonging to the axes
            bounds = [%inf  %inf  %inf ; -%inf  -%inf  -%inf]
            for i = 1:nc
                c = e.children(i)
                bounds = buildEnclosingBounds(c, bounds)
            end
            if and(bounds([5 6])==[%inf ; -%inf]) then
                bounds(:,3) = []
            end
        case "Compound"
            for i = 1:length(e.children)
                c = e.children(i)
                bounds = buildEnclosingBounds(c, bounds)
            end
        // ---------------------------------------------------------
        case "Polyline"
            data = e.data
            bounds(1,1) = min([data(:,1) ; bounds(1,1)])
            bounds(2,1) = max([data(:,1) ; bounds(2,1)])
            bounds(1,2) = min([data(:,2) ; bounds(1,2)])
            bounds(2,2) = max([data(:,2) ; bounds(2,2)])
            if (size(data,"c") > 2) then
                bounds(1,3) = min([data(:,3) ; bounds(1,3)])
                bounds(2,3) = max([data(:,3) ; bounds(2,3)])
            end
        // ---------------------------------------------------------
        case "Arc"
            data = e.data
            bounds(1,1) = min([data(1) ; bounds(1,1)]);
            bounds(2,1) = max([data(1)+data(3) ; bounds(2,1)]);
            bounds(1,2) = min([data(2)-data(4) ; bounds(1,2)]);
            bounds(2,2) = max([data(2) ; bounds(2,2)]);
        // ---------------------------------------------------------
        case "Rectangle"
            data = e.data
            bounds(1,1) = min([data(1) ; bounds(1,1)]);
            bounds(2,1) = max([data(1)+data(3) ; bounds(2,1)]);
            bounds(1,2) = min([data(2)-data(4) ; bounds(1,2)]);
            bounds(2,2) = max([data(2) ; bounds(2,2)]);
        // ---------------------------------------------------------
        case "Segs"
            data = e.data
            bounds(1,1) = min([data(:,1) ; bounds(1,1)]);
            bounds(2,1) = max([data(:,1) ; bounds(2,1)]);
            bounds(1,2) = min([data(:,2) ; bounds(1,2)]);
            bounds(2,2) = max([data(:,2) ; bounds(2,2)]);
            if (size(data,"c") > 2) then
                bounds(1,3) = min([data(:,3) ; bounds(1,3)]);
                bounds(2,3) = max([data(:,3) ; bounds(2,3)]);
            end
        // ---------------------------------------------------------
        case "Champ"
            data = e.data
            bounds(1,1) = min([data.x ; bounds(1,1)]);
            bounds(2,1) = max([data.x ; bounds(2,1)]);
            bounds(1,2) = min([data.y ; bounds(1,2)]);
            bounds(2,2) = max([data.y ; bounds(2,2)]);
        // ---------------------------------------------------------
        case "Surface"
            bounds = buildEnclosingBoundsOfSurface(e, bounds)
        case "Fac3d"
            bounds = buildEnclosingBoundsOfSurface(e, bounds)
        case "Plot3d"
            bounds = buildEnclosingBoundsOfSurface(e, bounds)
        case "Grayplot"
            bounds = buildEnclosingBoundsOfSurface(e, bounds)
        // ---------------------------------------------------------
        case "Fec"
            data = e.data
            // TODO: Fec zoom behavior is not clear to me, should be reviewed carefully
            bounds(1,1) = min([data(:,2) ; bounds(1,1)]);
            bounds(2,1) = max([data(:,2) ; bounds(2,1)]);
            bounds(1,2) = min([data(:,1) ; bounds(1,2)]);
            bounds(2,2) = max([data(:,1) ; bounds(2,2)]);
        // ---------------------------------------------------------
        case "Matplot"
            data = e.rect
            bounds(1,1) = min([data(1) ; bounds(1,1)]);
            bounds(2,1) = max([data(3) ; bounds(2,1)]);
            bounds(1,2) = min([data(2) ; bounds(1,2)]);
            bounds(2,2) = max([data(4) ; bounds(2,2)]);

        // ---------------------------------------------------------
        case "Text"
            data = e.data
            bounds(1,1) = min([data(1) ; bounds(1,1)]);
            bounds(1,2) = min([data(2) ; bounds(1,2)]);
            if (size(data,"c") > 2) then
                bounds(1,3) = min([data(3) ; bounds(1,3)]);
            end
        // ---------------------------------------------------------
        case "Legend"
            // Nothing to do: a legend is located in window relative coordinates,
            //                not in data coordinates
        // ---------------------------------------------------------
        case "Axis"
            bounds(1,1) = min([e.xtics_coord , bounds(1,1)]);
            bounds(2,1) = max([e.xtics_coord , bounds(2,1)]);
            bounds(1,2) = min([e.ytics_coord , bounds(1,2)]);
            bounds(2,2) = max([e.ytics_coord , bounds(2,2)]);
        // ---------------------------------------------------------
        case "Light"
            // nothing to do: a light source is never visible by itself
        // ---------------------------------------------------------
    else
        msg = _("%s: Handle of unsupported type ''%s'' as Argument #%d\n")
        error(msprintf(msg, "buildEnclosingBounds", e.type, 1))
    end
endfunction

// ===========================================================================

function [bounds] = buildEnclosingBoundsOfSurface(e, bounds)
    data = e.data
    bounds(1,1) = min([min(data.x) ; bounds(1,1)]);
    bounds(2,1) = max([max(data.x) ; bounds(2,1)]);
    bounds(1,2) = min([min(data.y) ; bounds(1,2)]);
    bounds(2,2) = max([max(data.y) ; bounds(2,2)]);
    bounds(1,3) = min([min(data.z) ; bounds(1,3)]);
    bounds(2,3) = max([max(data.z) ; bounds(2,3)]);
endfunction
