// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Bruno Pincon
// Copyright (C) 2010 - Samuel Gougeon
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - A. Khorshidi (new option)
// Copyright (C) 2013 - Scilab Enterpriss - Paul Bignier: added output
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [y, ind] = histplot(n,data,style,strf,leg,rect,nax,logflag,frameflag,axesflag,normalization,polygon)
    // histplot(n,data,<opt_arg_seq>)
    // draws histogram of entries in  data put into n classes
    //
    // histplot(xi,data,<opt_arg_seq>)
    // generates the histogram of entries in data put into classes
    // [xi(1),xi(2)], (xi(k) xi(k+1)], k=2,..,n.
    // xi's are assumed st. increasing (this point is verified now).
    //
    // optional args:
    //     1/ the same than for a plot2d:
    //        style,strf,leg,rect,nax,logflag,frameflag,axesflag
    //     2/ normalization flag (default value %t). When true the
    //        histogram is normalized so that to approach a density:
    //          xmax
    //         /
    //         |  h(x) dx = 1   (true if xmin <= min(data) and max(data) <= xmax)
    //         /
    //         xmin
    //
    // Example : enter histplot()
    //
    // modifs to use dsearch (Bruno Pincon 10/12/2001)
    // others modifs from Bruno (feb 2005):
    //    - may be a cleaner and shorter way to deal with optional arg ?
    //    - now the histgram is drawn using only one polyline
    //      (so properties (color, thickness,...) are easier
    //       to change with new graphics).
    //    - removed computation of nax and rect if they are not
    //      passed (let plot2d doing it)
    //    - modify a little the demo
    //    - add some checking on n|x and data
    //

    [lhs, rhs] = argn()

    y = [];
    ind = [];
    if rhs == 0 then   // demo
        histplot([-4.5:0.25:4.5],rand(1,20000,"n"),style=2,axesflag=1,..
        frameflag=1,rect=[-4.5 0 4.5 0.47]);
        deff("[y]=f(x)","y=exp(-x.*x/2)/sqrt(2*%pi);");
        x=-4.5:0.125:4.5;
        x=x';
        plot2d(x,f(x),26,"000");
        titre= gettext("histplot() : (normalized) histogram plot");
        xtitle(titre,"C (Classes)","N(C) / (Nmax length(C))");  // Not clear
        legend(gettext("Gaussian random sample histogram"), ..
        gettext("Exact gaussian density"));
        return
    end

    if rhs < 2
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"histplot",2));
    end

    if type(n) ~= 1 |  ~isreal(n)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real expected.\n"),"histplot",1));
    elseif type(data) ~= 1 | ~isreal(data)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real expected.\n"),"histplot",2));
    end

    // This is the only specific optional argument for histplot
    if exists("normalization","l")==0, normalization=%t,end
    if exists("polygon","l")==0, polygon=%f,end

    // Now parse optional arguments to be sent to plot2d
    opt_arg_seq = "";
    opt_arg_list = ["style","strf","leg","rect","nax","logflag","frameflag","axesflag"]
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
        end
    end
    [y, ind] = histc(n, data, normalization);

    if length(n) == 1 then  // The number of classes is provided
        x = linspace(min(data), max(data), n+1); // Class boundary
    else // The classes are provided
        x = matrix(n,1,-1);   // Force row form
        n = length(x)-1
    end

    if polygon then
        xmid=(x(1:$-1)+x(2:$))/2;...
        xp=[x(1)-(x(2)-x(1))/2 xmid x($)+(x($)-x($-1))/2];...
        yp=[0 y 0];
    end // new lines

    // now form the polyline
    //    X = [x1 x1 x2 x2 x2 x3 x3 x3  x4 ...   xn xn+1 xn+1]'
    //    Y = [0  y1 y1 0  y2 y2 0  y3  y3 ... 0 yn yn   0 ]'
    X = [x(1);x(1);matrix([1;1;1]*x(2:n),-1,1);x(n+1);x(n+1)]
    // BUG#1885
    // We start the histplot line to %eps rather than 0
    // So when switching to logarithmic mode we do not fall
    // in log(0) special behaviour.
    Y = [matrix([%eps;1;1]*y,-1,1);%eps]

    if opt_arg_seq == [] then
        plot2d(X,Y)
        if polygon then plot(xp,yp,"b-o"), end // new line
    else
        execstr("plot2d(X,Y"+opt_arg_seq+")")
        if polygon then plot(xp,yp,"r-o"), end // new line
    end
    e = gca();
    if polygon then
        e = e.children(2).children
    else
        e = e.children(1).children
    end
    e.fill_mode = "on";
    e.data(:,3) = -0.1;  // unmask the x-axis
    c = e.foreground;
    f = gcf();
    c = f.color_map(max(1,c),:)
    e.background = addcolor(1-(1-c)/20); // default filling color = edges one but fainter
endfunction
