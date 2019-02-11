// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Sfgrayplot(x, y, f, strf, rect, nax, zminmax, colminmax, mesh, colout)

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
    // Copyright INRIA
    // rewritten by Bruno Pincon (17 april 05) (based of my modifs of Sgrayplot)

    [lhs,rhs]=argn()
    if rhs == 0 then   // demo
        deff("[z]=Surf(x,y)","z=x.^3+y");
        f=gcf();
        f.color_map = jetcolormap(64);
        f.immediate_drawing = "off";
        colorbar(-2,2);
        Sfgrayplot(-1:0.1:1,-1:0.1:1,Surf,strf="031",rect=[-1,-1,1,1]);
        xtitle("$\Large f(x,y)=x^3+y$");
        f.immediate_drawing = "on";
        return
    elseif rhs < 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "Sfgrayplot", 3, 10));
    end

    // some checks
    if ~(type(x)==1 & isreal(x)) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"), "Sfgrayplot", 1))
    end
    if ~(type(y)==1 & isreal(y)) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"), "Sfgrayplot", 2))
    end
    if type(f)~=13 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: function expected.\n"), "Sfgrayplot", 3));
    end

    p = length(x);
    q = length(y);
    z = feval(x,y,f);

    // http://bugzilla.scilab.org/15638 :
    if ~isdef("colminmax","l")
        colminmax = [1, size(gcf().color_map,1)]
    end
    nbc = colminmax(2)-colminmax(1)+1
    if ~isdef("zminmax","l") then
        zminmax = [min(z), max(z)]
    end
    dz = (zminmax(2) - zminmax(1))/nbc/2
    zminmax = zminmax + [dz -dz]

    // parsing the optional args
    opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"];
    opt_arg_seq = "";
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg;
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
        fec(noe_x,noe_y,connect,z);
    else
        execstr("fec(noe_x,noe_y,connect,z"+opt_arg_seq+")");
    end
endfunction
