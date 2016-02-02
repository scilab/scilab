// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function hallchart(modules,args,colors)
    defaultmodules=[-20 -10 -6 -4 -2 2 4 6 10 20];//in dB
    defaultargs=[-90 -60 -45 -30 -15 15 30 45 60 90]; //in degree
    defaultbounds=[-3.5 -2;3 2];
    if exists("modules","local")==0 then
        modules=defaultmodules
    else
        if type(modules)|~isreal(modules)<>1 then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","modules");
        end
        modules=matrix(modules,1,-1)
    end
    if exists("args","local")==0 then
        args=defaultargs
    else
        if type(args)<>1|~isreal(args) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","args");
        end
        args=matrix(args,1,-1)
    end

    if exists("colors","local")==0 then
        colors=[4 12];
    else
        if type(colors)<>1|~isreal(colors) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","colors");
        end
        if size(colors,"*")==1 then
            colors=colors*ones(1,2)
        end
    end

    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    ax=gca();
    nc=size(ax.children,"*")
    if nc==0 then
        ax.data_bounds=defaultbounds;
        ax.axes_visible="on";
        ax.x_label.text=_("Real axis");
        ax.y_label.text=_("Imaginary axis");
        ax.title.text=_("Hall chart")
        ax.box="on";
    end

    //iso modules circles. Circles whose center are (-M^2/(M^2-1),0) and
    //radius M/(M^2-1) with M=|H(jw)| and H=G/(1+G)

    M=exp(log(10)*modules/20)
    radius=M./(M.*M-ones(M))
    xc=-M.*radius
    yc=0
    radius=abs(radius)
    //arcs replaced by polylines to be able to use datatips
    //  xarcs([xc-radius;yc+radius;2*radius;2*radius;0*M;360*64*ones(M)])
    //  A=gce()
    //  E=unglue(A);
    w=linspace(0,2*%pi,200)
    c=cos(w);s=sin(w)
    chart_handles=[]
    for i=1:size(M,"*")
        xpoly(xc(i)+radius(i)*c,yc+radius(i)*s)
        ec=gce();
        ec.foreground=colors(1);
        ec.line_style=7;
        ec.clip_state="clipgrf";
        ec.display_function = "formatHallModuleTip";
        ec.display_function_data = modules(i);
        if 2*int(i/2)==i then
            xs=xc(i)+radius(i)*cos(%pi/6)
            ys=yc+radius(i)*sin(%pi/6)
        else
            xs=xc(i)+radius(i)*cos(-%pi/6)
            ys=yc+radius(i)*sin(-%pi/6)

        end

        xstring(xs,ys,string(modules(i))+_("dB"));
        el=gce();

        el.font_foreground=colors(1);
        el.clip_state="clipgrf";
        chart_handles=[glue([el ec]) chart_handles];
    end

    //iso phase circles. Circles whose center are (-1/2, 1/(2*N)) and
    //radius sqrt(1+N^2)/(2*N) with N=tan(arg(H(-jw)))

    N=tan(args*%pi/180);
    radius=sqrt(1+N.^2)./(2*N);
    xc=-1/2;
    yc=1 ./(2*N);

    //  xarcs([xc-radius;yc+radius;2*radius;2*radius;0*N;360*64*ones(N)])
    //  E=unglue(gce());
    for i=1:size(N,"*")
        xpoly(xc+radius(i)*c,yc(i)+radius(i)*s);ec=gce();
        ec.foreground=colors(2);
        ec.line_style=7;
        ec.clip_state="clipgrf";
        ec.display_function = "formatHallPhaseTip";
        ec.display_function_data = args(i);
        xstring(xc,yc(i)+radius(i),msprintf("%g°",args(i)));
        el=gce();
        el.font_foreground=colors(2);
        el.clip_state="clipgrf";
        chart_handles=[glue([el ec]) chart_handles];
    end
    chart_handles=glue(chart_handles)
    //reorder axes children to make chart drawn before the previously
    // drawn curves if any
    for k=1:nc
        swap_handles(ax.children(k),ax.children(k+1))
    end
    fig.immediate_drawing=immediate_drawing;
endfunction
