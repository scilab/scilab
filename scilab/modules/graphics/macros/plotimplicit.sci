// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - SCILAB ENTERPRISES - Claude Gomez
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
// Copyright (C) 2019-2020 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// ===================================================================

function plotimplicit(fun, x_grid, y_grid, varargin)

    fname = "plotimplicit"
    [lhs, rhs] = argn(0);

    if rhs == 0 then
        ab = gca().axes_bounds
        delete(gca())
        xsetech(ab)
        plotimplicit("rand() = 0.5", 1:10, 1:10);
        return
    end

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    if winsid()<>[] then
        db = gca().data_bounds
    else
        db = gda().data_bounds
    end
    // x_grid
    // --------
    if ~isdef("x_grid","l") then
        x_grid = linspace(-1, 1, 201);
    else
        if type(x_grid) <> 1 & ..
            (typeof(x_grid)<> "implicitlist" | x_grid <> :) then
            error(msprintf(gettext("%s: Argument #%d: Real vector or colon : expected.\n"), fname, 2));
        end
        if typeof(x_grid)=="implicitlist" & x_grid == :
            //if ~isdef("y_grid","l")
            //    y_grid = :
            //end
            x_grid = db(1:2)
        end
        if ~isvector(x_grid) then
            error(msprintf(gettext("%s: Argument #%d: Vector expected.\n"), fname, 2));
        end
        if ~isreal(x_grid) then
            error(msprintf(gettext("%s: Argument #%d: Real value expected.\n"), fname, 2));
        end
        if length(x_grid)==2
            x_grid = linspace(min(x_grid), max(x_grid), 201)
        end
    end

    // y_grid
    // --------
    if ~isdef("y_grid","l") then
        y_grid = x_grid;
    else
        if type(y_grid) <> 1 & ..
            (typeof(y_grid)<> "implicitlist" | y_grid <> :)then
            error(msprintf(gettext("%s: Argument #%d: Real vector or colon : expected.\n"), fname, 3));
        end
        if typeof(y_grid)=="implicitlist" & y_grid == :
            y_grid = db(3:4)
        end
        if ~isvector(y_grid) then
            error(msprintf(gettext("%s: Argument #%d: Vector expected.\n"), fname, 3));
        end
        if ~isreal(y_grid) then
            error(msprintf(gettext("%s: Argument #%d: Real value expected.\n"), fname, 3));
        end
        if length(y_grid)==2
            y_grid = linspace(min(y_grid), max(y_grid), 201)
        end
    end

    // Implicit function | equation
    // ----------------------------
    // fun is a string (literal expression of a function),
    //        a function handle,
    //        a list(fun, parameters(:))
    fun_type = type(fun)
    fun0 = fun
    if ~or(fun_type == [10 13 15 130]) then
        error(msprintf(gettext("%s: Argument #%d: Real matrix or String or Scilab function or list expected.\n"), fname, 1));
    elseif fun_type == 10
        fun = fun(:)
        fun(fun=="") = []
        if size(fun, "*") ~= 1
            error(msprintf(gettext("%s: Argument #%d: Scalar (1 element) expected.\n"), fname, 1));
        end
        fun = tokens(fun, "=")
        if size(fun,1)==2
            fun = fun(1) + " - (" + fun(2) + ")"
        end
        // Vectorizing * / ^ => .* ./ .^
        for o = ["*" "/" "^"]
            fun = strsubst(fun, "."+o, "€£")
            fun = strsubst(fun, o, "."+o)
            fun = strsubst(fun, "€£", "."+o)
        end
        deff("z = internal_fun(x, y)", "z = " + fun);

    elseif or(type(fun)==[13 130])
        internal_fun = fun

    elseif type(fun)==15  // list(fun, params(:))
        internal_fun = fun(1)
        if and(type(internal_fun) <> [13 130])
            msg = gettext("%s: Argument #%d(#%d): Identifier of a Scilab or built-in function expected.\n")
            error(msprintf(msg, fname, 1, 1))
        end
        if size(fun) > 1
            params = fun
            params(1) = null()
        end
    end

    // Computation of Z = internal_fun(x,y,..)
    // ---------------------------------------
    // We try in a vectorized way
    try
        nx = min(size(x_grid,"*"),2)
        ny = min(size(y_grid,"*"),3)
        [x, y] = ndgrid(x_grid(1:nx), y_grid(1:ny))
        if ~isdef("params","l")
            z = internal_fun(x, y);
            if or(size(z) <> [2 3]) then
                z = feval(x_grid, y_grid, internal_fun)
            else
                [x, y] = ndgrid(x_grid, y_grid)
                z = internal_fun(x, y)
            end
        else
            z = internal_fun(x, y, params(:))
            if or(size(z) <> [2 3]) then
                z = feval(x_grid, y_grid, fun)
            else
                [x, y] = ndgrid(x_grid, y_grid)
                z = internal_fun(x, y, params(:))
            end
        end
        clear x y
    catch
        // internal_fun() is not vectorized => we call feval()
        if ~isdef("params","l")
            z = feval(x_grid, y_grid, internal_fun)
        else
            z = feval(x_grid, y_grid, fun)
        end
    end


    // PLOT NITIALIZATION
    // ------------------
    fig = gcf();
    ax = gca();

    v = fig.immediate_drawing;
    fig.immediate_drawing = "off";


    // COMPUTING ROOT CONTOURS
    // -----------------------
    [xc, yc] = contour2di(x_grid, y_grid, z, [0, 0]);
    clear z

    // xc and yc might be empty: no zeros were found in x_grid X y_grid
    cnt = 0
    if size(xc, "*") == 0
        warning(msprintf(gettext("%s: Function evaluation in selected range does not cross any zeros.\n"), fname));
        plotframe([min(x_grid) min(y_grid) max(x_grid) max(y_grid)])

    else
        // Cleaning results of contour2di
        // The function might return several times the same contour
        k = 1;
        i = 1;
        list_coord = list();
        n = yc(k);
        list_coord(i) = [xc(k + (1:n)); yc(k + (1:n))];
        k = k + n + 1;

        while k < length(xc)
            n = yc(k);
            exist_in_list = %f;
            new_coord = [xc(k + (1:n)); yc(k + (1:n))];

            for it = 1:i
                if and(list_coord(it) == new_coord)
                    exist_in_list = %t;
                    break;
                end
            end
            if ~exist_in_list
                i = i + 1;
                list_coord(i) = new_coord;
            end
            k = k + n + 1;
        end

        // Now list_coord contains each path for the contour drawing
        // Draw each polyline element inside a compound
        for it = 1:i
            err = execstr("plot(list_coord(it)(1,:), list_coord(it)(2,:), varargin(:))", "errcatch", "m");

            // add a test to see if plot2d call succeed
            if err <> 0
                mprintf(gettext("Error %d : in plot called by plotimplicit"), err);
            else
                // unfold the Compound created after plot into its Polyline content
                unglue(ax.children(1))
                cnt = cnt + 1;
            end
        end

        if cnt > 0 then
            // glue all Polylines together into a Compound
            a = glue(ax.children(1:cnt));
            // Otherwise: a remains the axes handle
        end
    end


    // Titles
    if ax.x_label.text=="" then
        xlabel("$x$", "fontsize",4)
    end
    if ax.y_label.text=="" then
        ylabel("$y$", "fontsize",4)
    end
    if ax.title.text=="" & fun_type==10 then
        if grep(fun0, "=")==[]
            fun0 = fun0 + "= 0"
        end
        fun0 = strsubst(fun0, ".*", "\cdot ")
        fun0 = strsubst(fun0, "*", "\cdot ")
        fun0 = strsubst(fun0, "./", "/")
        //fun0 = strsubst(fun0, "^ [^\s]+ ", "^{}")
        title("$"+fun0+"$", "fontsize",4)
    end
    //
    if cnt > 0 then
        set("current_entity", a);
    else
        set("current_entity", ax);
    end
    fig.immediate_drawing = v;
endfunction
