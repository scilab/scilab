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

function sgrid(varargin)
    // sgrid(["new",] [,Z,Wn [,colors]])
    // sgrid(Z,Wn [,"new"] [,colors])

    defaultcolors=[4 12];
    defaultbounds=[-1.6,-1.6;0,1.6];
    rhs=argn(2)
    new=%f
    if rhs>=1 then
        if type(varargin(1))==10 then
            if varargin(1)<>"new" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"sgrid",1,"new"))
            end
            varargin(1)=null()
            rhs=rhs-1
            new=%t
        end
    end
    if rhs>=3 then
        if type(varargin(3))==10 then
            if varargin(3)<>"new" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"sgrid",3,"new"))
            end
            varargin(3)=null()
            rhs=rhs-1
            new=%t
        end
    end

    defaultwn=%f;
    defaultzeta=[0 0.16 0.34 0.5 0.64 0.76 0.86 0.94 0.985 1]

    select rhs
    case 0 then  //sgrid() or sgrid("new")
        defaultwn=%t
        zeta = defaultzeta
        colors = defaultcolors
    case 2 then
        zeta =  varargin(1)
        if type(zeta)<>1|~isreal(zeta) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"sgrid",1));
        end
        wn = varargin(2)
        if type(wn)<>1|~isreal(wn) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"sgrid",2));
        end
        colors = defaultcolors
    case 3 then
        zeta =  varargin(1)
        if type(zeta)<>1|~isreal(zeta) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"sgrid",1));
        end
        wn = varargin(2)
        if type(wn)<>1|~isreal(wn) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"sgrid",2));
        end
        colors = varargin(3)
        if type(colors)<>1|~isreal(colors) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"sgrid",3));
        end
        if size(colors,"*")==1 then colors=colors*ones(1,2),end
    end
    zeta=zeta(zeta>=0&zeta<=1)
    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    axes=gca();
    if new&axes.children<>[] then
        delete(axes.children)
    end
    nc=size(axes.children,"*")
    if nc==0 then
        axes.data_bounds=defaultbounds
        axes.axes_visible="on";
        ax.box="on";
    else
        axes.data_bounds=[min(defaultbounds(1,:),axes.data_bounds(1,:));
        max(defaultbounds(2,:),axes.data_bounds(2,:))];
    end
    if axes.tight_limits=="off"&axes.auto_ticks(1)=="on" then
        xmin=axes.x_ticks.locations(1)
        xmax=axes.x_ticks.locations($)
    else
        xmin=axes.data_bounds(1,1)
        xmax=axes.data_bounds(2,1)
    end
    if axes.tight_limits=="off"&axes.auto_ticks(2)=="on" then
        ymin=axes.y_ticks.locations(1)
        ymax=axes.y_ticks.locations($)
    else
        ymin=axes.data_bounds(1,2)
        ymax=axes.data_bounds(2,2)
    end

    //iso natural frequency curves
    if defaultwn then
        rmax=sqrt(min(xmin^2+ymin^2,xmin^2+ymax^2))
        rmax=floor(10*rmax)/10;
        [xi,xa,np]=graduate(0.2,rmax,8,12)
        wn=linspace(xi,xa,np)
    end
    wn=wn(wn>=0)
    t=linspace(%pi/2,1.5*%pi,200)
    chart_handles=[]
    for i=1:size(wn,"*")
        xpoly(wn(i)*cos(t),wn(i)*sin(t))
        ec=gce();
        ec.display_function = "formatSgridFreqTip";
        ec.display_function_data = wn(i);

        ec.foreground=colors(1),
        ec.line_style=7;
        ec.clip_state="clipgrf";

        lab=msprintf("%.2g",wn(i))
        r=xstringl(0,wn(i),lab)
        if %t then
            xstring(-r(3),wn(i),lab)
            es1=gce();
            es1.font_foreground=colors(1),
            es1.clip_state="clipgrf";
            xstring(-r(3),-wn(i)-r(4),lab)
            es2=gce();
            es2.font_foreground=colors(1),
            es2.clip_state="clipgrf";
            e=glue([es2 es1 ec])
        else
            xstring(-wn(i)-r(3),0,lab)
            es1=gce();
            es1.font_foreground=colors(1),
            es1.clip_state="clipgrf";
            e=glue([es1 ec]);
        end
        chart_handles=[e chart_handles]
    end
    //iso damping factor curves
    angl=acos(zeta)

    for i=1:size(zeta,"*")
        lab=string(zeta(i))
        rect=xstringl(0,0,lab)

        sa=sin(angl(i));ca=cos(angl(i))
        if sa==0 then sa=%eps;end
        if ca==0 then ca=%eps;end
        r=min(-ymin/sa,-xmin/ca)

        xpoly([0 -r*ca],[0 -r*sa])
        ec=gce();
        ec.display_function = "formatSgridDampingTip";
        ec.display_function_data = zeta(i);

        ec.foreground=colors(2),
        ec.line_style=7;
        ec.clip_state="clipgrf";
        if -ymin/sa<-xmin/ca
            r1=r-rect(4)/sa
            xstring(-r1*ca-rect(3),-r1*sa,lab)
        else
            xstring(-r*ca+0.02*rect(3),-r*sa,lab)
        end
        es=gce();
        es.font_foreground=colors(2),
        es.clip_state="clipgrf";
        chart_handles=[glue([es ec]),chart_handles]

        r=min(ymax/sa,-xmin/ca)
        xpoly([0 -r*ca],[0 r*sa])
        ec=gce();
        ec.display_function = "formatSgridDampingTip";
        ec.display_function_data = zeta(i);

        ec.foreground=colors(2),
        ec.line_style=7;
        ec.clip_state="clipgrf";
        if ymax/sa<-xmin/ca then
            xstring(-r*ca-rect(3),r*sa-rect(4),lab)
        else
            xstring(-r*ca,r*sa,lab)
        end
        es=gce();
        es.font_foreground=colors(2),
        es.clip_state="clipgrf";
        chart_handles=[glue([es ec]),chart_handles]
    end
    chart_handles=glue(chart_handles)
    //reorder axes children to make chart drawn before the black previously
    // drawn curves if any
    for k=1:nc
        swap_handles(axes.children(k),axes.children(k+1))
    end
    fig.immediate_drawing=immediate_drawing;
    show_window()
endfunction
