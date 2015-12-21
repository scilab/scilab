function []=Sfgrayplot(x, y, f, strf, rect, nax, zminmax, colminmax, mesh, colout)

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
    if type(f)~=11 & type(f)~=13 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: function expected.\n"), "Sfgrayplot", 3));
    end

    p = length(x); q = length(y);

    // parsing the optional args
    opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"];
    opt_arg_seq = "";
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg;
        end
    end

    // build the data for fec
    z = feval(x,y,f);
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
