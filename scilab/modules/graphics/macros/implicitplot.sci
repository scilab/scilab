// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - SCILAB ENTERPRISES - Claude Gomez
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

function implicitplot(fun, x_range, y_range, varargin)

    [lhs, rhs] = argn(0);

    if rhs == 0 then
        scf();
        implicitplot("rand() - 0.5", 1:10, 1:10);
        return;
    end

    // x and y must be constants
    if rhs == 1 then
        x_range = linspace(-1, 1, 101);
        y_range = linspace(-1, 1, 101);
        varargin = [];
    end
    if rhs == 2 then
        y_range = linspace(-1, 1, 101);
        varargin = [];
    end

    if rhs >= 2 then
        if type(x_range) <> 1 then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot", 2));
        end
        if ~isvector(x_range) then
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot", 2));
        end
        if ~isreal(x_range) then
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot", 2));
        end
    end

    if rhs >= 3 then
        if type(y_range) <> 1 then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot", 3));
        end
        if ~isvector(y_range) then
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot", 3));
        end
        if ~isreal(y_range) then
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot", 3));
        end
    end

    // fun is a string or a function
    fun_type = type(fun);
    if ~or(fun_type == [1 10 11 13]) then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix or String or Scilab function expected.\n"), "implicitplot", 1));
    end

    if fun_type == 1 then
        if or(size(fun) ~= [length(x_range), length(y_range)])
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "implicitplot", 1, length(x_range), length(y_range)));
        end
    else
        if fun_type == 10
            if size(fun, "*") ~= 1
                error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Scalar string expected.\n"), "implicitplot", 1));
            end
        end
    end

    // Do not signal INF and NAN
    old_ieee = ieee();
    ieee(2);

    if type(fun) ~= 1
        // Define the function
        if type(fun) == 10 then
            deff("[%z] = internal_fun(x, y)", "%z = " + fun);
        else
            internal_fun = fun;
            if type(fun) == 11 then
                comp(internal_fun);
            end
        end
        %z = feval(x_range, y_range, internal_fun);
    else
        %z = fun;
    end

    [xc, yc] = contour2di(x_range, y_range, %z, [0, 0]);
    clear z;

    // xc and yc might be empty: no zeros were found in x_range X y_range
    if size(xc, "*") == 0
        error(999, msprintf(gettext("%s: Function evaluation in selected range does not cross any zeros.\n"), "implicitplot"));
    end

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

    clear xc, yc;
    // Now list_coord contains each path for the contour drawing
    // Draw each polyline element inside a compound
    fig = gcf();
    a = gca();

    v = fig.immediate_drawing;
    fig.immediate_drawing = "off";
    cnt = 0;

    for it = 1:i

        err = execstr("plot(list_coord(it)(1,:), list_coord(it)(2,:), varargin(:))", "errcatch", "m");

        // add a test to see if plot2d call succeed
        if err <> 0
            mprintf(gettext("Error %d : in plot called by implicitplot"), err);
            fig.immediate_drawing = v;
            clear list_coord
            return;
        end

        // unfold the Compound created after plot into its Polyline content
        unglue(a.children(1))
        cnt = cnt + 1;
    end

    if cnt > 0 then
        // glue all Polylines together into a Compound
        glue(a.children(1:cnt));
    end
    set("current_entity", a);
    fig.immediate_drawing = v;
    ieee(old_ieee);
    clear list_coord
endfunction
