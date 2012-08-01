// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Bruno Pincon
// Copyright (C) 2010 - Samuel Gougeon
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function histplot(n,data,style,strf,leg,rect,nax,logflag,frameflag,axesflag,normalization)
    // histplot(n,data,<opt_arg_seq>)
    // draws histogram of entries in  data put into n classes
    //
    // histplot(xi,data,<opt_arg_seq>)
    // generates the histogram of entries in data put into classes
    // [xi(1),xi(2)], (xi(k) xi(k+1)], k=2,..,n.
    // xi's are assumed st. increasing (this point is verified now).
    //
    // optionnal args:
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
    //    - may be a cleaner and shorter way to deal with optionnal arg ?
    //    - now the histgram is drawn using only one polyline
    //      (so properties (color, thickness,...) are easier
    //       to change with new graphics).
    //    - removed computation of nax and rect if they are not
    //      passed (let plot2d doing it)
    //    - modify a little the demo
    //    - add some checking on n|x and data
    //
    [lhs,rhs]=argn()

    if rhs == 0 then   // demo
        histplot([-4.5:0.25:4.5],rand(1,20000,'n'),style=2,axesflag=1,..
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
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real  expected.\n"),"histplot",1));
    elseif type(data) ~= 1 | ~isreal(data)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real  expected.\n"),"histplot",2));
    end

    // this is the only specific optionnal argument for histplot
    if ~exists('normalization','local') then, normalization=%t,end

    // now parse optionnal arguments to be sent to plot2d
    opt_arg_seq = []
    opt_arg_list = ["style","strf","leg","rect","nax","logflag","frameflag","axesflag"]
    for opt_arg = opt_arg_list
        if exists(opt_arg,"local") then
            opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
        end
    end

    p = length(data)
    if length(n) == 1 then  // the number of classes is provided
        if n < 1
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"histplot",1,"[1, oo)"));
        elseif n~=floor(n)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Integer expected.\n"),"histplot",1))
        end
        mind = min(data);
        maxd = max(data);
        if (mind == maxd) then
            mind = mind - floor(n/2); 
            maxd = maxd + ceil(n/2);
        end
        x = linspace(mind, maxd, n+1);
    else                    // the classes are provided
        x = matrix(n,1,-1)   // force row form
        if min(diff(x)) <= 0 then
            error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"),"histplot",1))
        end
        n = length(x)-1
    end

    [ind , y] = dsearch(data, x)

    if normalization then y=y ./ (p *(x(2:$)-x(1:$-1))),end

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
    else
        execstr('plot2d(X,Y'+opt_arg_seq+")")
    end

endfunction





