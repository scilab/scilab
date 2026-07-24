// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = Sfgrayplot(x, y, f, strf, rect, nax, zminmax, colminmax, mesh, colout)

    // PURPOSE
    //    Like fgrayplot but the function fec is used to smooth the
    //    result assuming that the underlying function is linear on
    //    a set of triangles built from the grid (here with n1=5, n2=3):
    //             _____________
    //             | /| /| /| /|
    //             |/_|/_|/_|/_|
    //             | /| /| /| /|
    //             |/_|/_|/_|/_|
    //
    //    Two calling syntaxes are supported:
    //
    //    Rectangular domain (vectors x, y):
    //      Sfgrayplot(x, y, f) where x is a vector of length n, y of length m,
    //      and f is a function with z(i,j) = f(x(i), y(j)).
    //      The function is evaluated via feval(x, y, f).
    //
    //    Non-rectangular domain (matrices X, Y):
    //      Sfgrayplot(X, Y, f) where X, Y are matrices of the same size (n,m)
    //      and f is a vectorized function such that Z = f(X, Y) returns a
    //      matrix of the same size with Z(i,j) = f(X(i,j), Y(i,j)).
    //
    // Copyright INRIA
    // rewritten by Bruno Pincon (17 april 05) (based of my modifs of Sgrayplot)

    [lhs,rhs]=argn()
    if rhs == 0 then   // demo
        deff("[z]=Surf(x,y)","z=x.^3+y");
        f=gcf();
        f.color_map = jet(64);
        f.immediate_drawing = "off";
        colorbar(-2,2);
        Sfgrayplot(-1:0.1:1,-1:0.1:1,Surf,strf="031",rect=[-1,-1,1,1]);
        xtitle("$\Large f(x,y)=x^3+y$");
        f.immediate_drawing = "on";
        return
    elseif rhs < 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "Sfgrayplot", 3, 10));
    end

    if lhs > 1
        msg = gettext("%s: Wrong number of output arguments: At most %d expected.\n")
        error(msprintf(msg, "Sfgrayplot", 1));
    end

    // type checks
    if ~(type(x)==1 & isreal(x)) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"), "Sfgrayplot", 1))
    end
    if ~(type(y)==1 & isreal(y)) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"), "Sfgrayplot", 2))
    end
    if type(f)~=13 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: function expected.\n"), "Sfgrayplot", 3));
    end

    if min(size(x)) > 1 | min(size(y)) > 1 then
        // ---- Non-rectangular domain: X, Y are matrices of the same size (p,q) ----
        [p, q] = size(x);
        if ~isequal(size(y), [p q]) then
            msg = gettext("%s: Wrong size for input arguments: Arguments #%d and #%d must have the same size.\n")
            error(msprintf(msg, "Sfgrayplot", 1, 2));
        end
        // Evaluate f element-wise: f must accept and return matrices of size (p,q)
        z = f(x, y);
        if ~isequal(size(z), [p q]) then
            msg = gettext("%s: The function #%d must return a matrix of the same size as arguments #%d and #%d.\n")
            error(msprintf(msg, "Sfgrayplot", 3, 1, 2));
        end
        // Use the X, Y matrices directly as node coordinates
        noe_x = x;
        noe_y = y;
    else
        // ---- Rectangular domain: x, y are vectors ----
        p = length(x);
        q = length(y);
        // Evaluate f on the full rectangular grid
        z = feval(x,y,f);
        // Build the full node coordinate arrays from the 1-D axis vectors
        [noe_x,noe_y] = ndgrid(x,y)
    end

    // https://gitlab.com/scilab/scilab/-/issues/15638 :
    if ~isdef("colminmax","l")
        colminmax = [1, size(gcf().color_map,1)]
    end
    nbc = colminmax(2)-colminmax(1)+1
    if ~isdef("zminmax","l") then
        zminmax = [min(z), max(z)]
    end

    // parsing the optional args
    opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"];
    opt_arg_seq = "";
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg;
        end
    end

    // Build the triangulation for fec.
    // For a (p x q) grid of nodes stored column-major, the node at position
    // (i,j) has linear index  i + (j-1)*p  (1-based).
    // Each quad (i,j)-(i+1,j)-(i,j+1)-(i+1,j+1) is split into two triangles:
    //   T1 : (i,j), (i+1,j), (i,j+1)   → indices  num, num1, num+p
    //   T2 : (i+1,j), (i+1,j+1), (i,j+1) → indices num1, num1+p, num+p
    // This is the same formula for both rectangular and non-rectangular grids.
    nbtri = 2*(p-1)*(q-1)
    num = (1:p*(q-1))'; num(p*(1:q-1)) = []; num1 = num+1
    connect =[(1:nbtri)' , [num   num1   num+p;...
    num1  num1+p num+p]  ,  zeros(nbtri,1)]

    // then plot
    if opt_arg_seq == "" then
        e = fec(noe_x,noe_y,connect,z);
    else
        execstr("e = fec(noe_x,noe_y,connect,z"+opt_arg_seq+")");
    end

    if lhs == 1
        varargout(1) = e;
    end

endfunction
