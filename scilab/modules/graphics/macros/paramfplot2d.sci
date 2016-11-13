// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function paramfplot2d(f,x,theta,flag,rect)
    //animated plot of x-->f(x,t) for t=theta(1),theta(2),etc
    //x=N-vector of x-values
    //f(x,t)=N-vector of y-values.
    //f: mapping x,t -> f(x,t) = R^N valued function for x= vector of R^N and t=real number.
    //f can be a either Scilab function or a dynamically linked routine since
    // y=f(x,t) is evaluated as y=feval(x(:),t,f). See feval.
    // Here y should be a column vector.
    // vector of parameters theta=[theta(1), theta(2),... theta(M)]
    // Optional parameters
    //flag = 'yes' (screen is cleared between two consecutive plots).
    //flag = 'no'  (screen is not cleared between two consecutive plots).
    //
    //rect = "rectangle" [xmin, xmax, ymin, ymax] (1 x 4 real vector),
    // containing a-priori lower and upper bounds for x and f(t,x).
    //function y=f(x,t),y=abs(cos(1.5*x+4*t)).*sin(x+10*t),endfunction
    //x=linspace(0,20*%pi,500);theta=0:0.05:5;
    [lhs,rhs]=argn(0)

    if ~rhs then
        deff("y=f(x,t)","y=t*sin(x)")
        x=linspace(0,2*%pi,50);theta=0:0.05:1;
        clf;
        paramfplot2d(f,x,theta);
        return;
    end

    if rhs<3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "paramfplot2d", 3));
    end

    x=x(:);
    theta=theta(:).'; // it should be a row-vector
    if rhs<5 then //compute the data bounds
        xmin=min(x);xmax=max(x);
        ymin=%inf;ymax=-%inf;
        for t=theta
            y=f(x,t); ymin=min(ymin,min(y)); ymax=max(ymax,max(y));
        end
        rect=[xmin,xmax,ymin,ymax];
    end
    if rhs<4 then flag="no";end
    realtimeinit(0.1);

    clf();
    fig=gcf();
    a=gca();
    a.data_bounds=matrix(rect,2,2);
    a.axes_visible="on";
    y=feval(x,theta(1),f);
    xpoly(x,y(:));p=gce(); //the polyline handle
    realtime(0);
    if flag=="no" then
        drawlater();
        for k=1:size(theta,"*")
            realtime(k);
            y=feval(x,theta(k),f);
            p.data(:,2)=y(:);
            drawnow();
        end
    else
        drawlater();
        for k=1:size(theta,"*")
            realtime(k);
            plot2d(x,feval(x,theta(k),f))
            drawnow();
        end
    end

endfunction
