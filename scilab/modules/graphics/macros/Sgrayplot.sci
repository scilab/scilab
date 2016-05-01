// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Bruno Pincon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=Sgrayplot(x,y,z, strf, rect, nax, zminmax, colminmax, mesh, colout)
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
    // Copyright INRIA
    // Modified by Bruno Pincon (14 oct 04) to have named argument working
    // Some new modifs (Bruno Pincon, Feb 2005, demo + some checking + cleaning + add
    // mesh and colout optional args)

    [lhs,rhs] = argn();

    if rhs == 0 then   // demo

        t=-%pi:0.1:%pi;
        m=sin(t)'*cos(t)
        f=gcf();
        f.color_map = jetcolormap(64);
        f.immediate_drawing = "off";
        colorbar(-1,1);
        Sgrayplot(t,t,m,strf="041",zminmax=[-1,1]);
        xtitle("Sgrayplot demo f(x,y)=sin(x)*cos(y) on [-pi,pi]x[-pi,pi]");
        f.immediate_drawing = "on";
        return

    elseif rhs < 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"Sgrayplot",3));
    end

    // some checks
    if ~(type(x)==1 & isreal(x) & type(y)==1 & isreal(y) & type(z)==1 & isreal(z)) then
        error(msprintf(gettext("%s: Wrong type for input argument(s): Arguments #%d, #%d and #%d must be real.\n"), "Sgrayplot",1,2,3));
    end
    nx = length(x); ny = length(y); [p,q] = size(z)
    if p ~= nx then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: The number of rows of argument #%d must be equal to the size of argument #%d.\n"),"Sgrayplot", 1, 3, 3, 1));
    elseif q~=ny then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: The number of columns of argument #%d must be equal to the size of argument #%d.\n"),"Sgrayplot", 2, 3, 3, 2));
    end

    // parsing the optional args
    opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"]
    opt_arg_seq = ""
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
        end
    end

    // build the data for fec
    [noe_x,noe_y] = ndgrid(x,y)
    nbtri = 2*(p-1)*(q-1)
    num = (1:p*(q-1))'; num(p*(1:q-1)) = []; num1 = num+1
    connect =[(1:nbtri)' , [num   num1   num+p;...
    num1  num1+p num+p]  ,  zeros(nbtri,1)]

    // then plot
    if opt_arg_seq == "" then
        fec(noe_x,noe_y,connect,z)
    else
        execstr("fec(noe_x,noe_y,connect,z"+opt_arg_seq+")")
    end
endfunction
