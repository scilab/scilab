// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

function varargout = Sgrayplot(x,y,z, strf, rect, nax, zminmax, colminmax, mesh, colout)
    //
    // PURPOSE
    //    Like grayplot but the function fec is used to smooth the
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
    //      Sgrayplot(x, y, z) where x is a vector of length n, y of length m,
    //      and z is a matrix of size (n,m) with z(i,j) = F(x(i), y(j)).
    //
    //    Non-rectangular domain (matrices X, Y, Z):
    //      Sgrayplot(X, Y, Z) where X, Y, Z are matrices of the same size (n,m)
    //      with Z(i,j) = F(X(i,j), Y(i,j)).
    //
    // Copyright INRIA
    // Modified by Bruno Pincon (14 oct 04) to have named argument working
    // Some new modifs (Bruno Pincon, Feb 2005, demo + some checking + cleaning + add
    // mesh and colout optional args)

    [lhs,rhs] = argn();

    if rhs == 0 then   // demo

        t=-%pi:0.1:%pi;
        m=sin(t)'*cos(t)
        f=gcf();
        f.color_map = jet(64);
        f.immediate_drawing = "off";
        colorbar(-1,1);
        Sgrayplot(t,t,m,strf="041",zminmax=[-1,1]);
        xtitle("Sgrayplot demo f(x,y)=sin(x)*cos(y) on [-pi,pi]x[-pi,pi]");
        f.immediate_drawing = "on";
        return

    elseif rhs < 3 then
        msg = gettext("%s: Wrong number of input arguments: At least %d expected.\n")
        error(msprintf(msg, "Sgrayplot", 3));
    end

    if lhs > 1
        msg = gettext("%s: Wrong number of output arguments: At most %d expected.\n")
        error(msprintf(msg, "Sgrayplot", 1));
    end

    // type checks
    if ~(type(x)==1 & isreal(x) & type(y)==1 & isreal(y) & type(z)==1 & isreal(z)) then
        msg = gettext("%s: Wrong type for input argument(s): Arguments #%d, #%d and #%d must be real.\n")
        error(msprintf(msg, "Sgrayplot", 1, 2, 3));
    end

    [p,q] = size(z)

    if min(size(x)) > 1 | min(size(y)) > 1 then
        // ---- Non-rectangular domain: X, Y, Z are matrices of the same size (p,q) ----
        if ~isequal(size(x), [p q]) ||  ~isequal(size(y), [p q]) then
            msg = gettext("%s: Wrong size for input arguments: Arguments #%d, #%d and #%d must have the same size.\n")
            error(msprintf(msg, "Sgrayplot", 1, 2, 3));
        end
        // Use the X, Y matrices directly as node coordinates
        noe_x = x;
        noe_y = y;
    else
        // ---- Rectangular domain: x, y are vectors ----
        nx = length(x); ny = length(y);
        if p ~= nx then
            msg = gettext("%s: Wrong size for input arguments #%d and #%d: The number of rows of argument #%d must be equal to the size of argument #%d.\n")
            error(msprintf(msg, "Sgrayplot", 1, 3, 3, 1));
        elseif q~=ny then
            msg = gettext("%s: Wrong size for input arguments #%d and #%d: The number of columns of argument #%d must be equal to the size of argument #%d.\n")
            error(msprintf(msg, "Sgrayplot", 2, 3, 3, 2));
        end
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
    opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"]
    opt_arg_seq = ""
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
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
         e = fec(noe_x,noe_y,connect,z)
    else
        execstr("e = fec(noe_x,noe_y,connect,z"+opt_arg_seq+")")
    end

    if lhs == 1
        varargout(1) = e;
    end

endfunction
