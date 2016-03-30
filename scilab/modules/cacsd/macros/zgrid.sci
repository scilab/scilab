// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid Belahcene
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=zgrid(varargin)
    // zgrid(["new",] [,Z,Wn [,colors]])
    // zgrid(Z,Wn [,"new"] [,colors])
    defaultcolors=[-1 -1];
    defaultbounds=[-1,-1;1,1];
    rhs=argn(2)
    new=%f
    if rhs>=1 then
        if type(varargin(1))==10 then
            if varargin(1)<>"new" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"zgrid",1,"new"))
            end
            varargin(1)=null()
            rhs=rhs-1
            new=%t
        end
    end
    if rhs>=3 then
        if type(varargin(3))==10 then
            if varargin(3)<>"new" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"zgrid",3,"new"))
            end
            varargin(3)=null()
            rhs=rhs-1
            new=%t
        end
    end

    defaultwn=0:0.1:1;
    defaultzeta=0:0.1:1;

    select rhs
    case 0 then  //zgrid() or zgrid("new")
        wn=defaultwn
        zeta = defaultzeta
        colors = defaultcolors
    case 1 then
        zeta =  varargin(1)
        if type(zeta)<>1|~isreal(zeta) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",1));
        end
        wn=defaultwn
        colors = defaultcolors
    case 2 then
        zeta =  varargin(1)
        if type(zeta)<>1|~isreal(zeta) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",1));
        end
        wn = varargin(2)
        if type(wn)<>1|~isreal(wn) then
            error(msprintf("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",2);
        end
        colors = defaultcolors
    case 3 then
        zeta =  varargin(1)
        if type(zeta)<>1|~isreal(zeta) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",1));
        end
        wn = varargin(2)
        if type(wn)<>1|~isreal(wn) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",2));
        end
        colors = varargin(3);
        if type(colors)<>1|~isreal(colors) then
            error(msprintf(_("%s: Wrong type for input argument #%d : real floating point array expected\n"),"zgrid",3));
        end
        if size(colors,"*")==1 then colors=colors*ones(1,2),end
    end
    wn=wn(wn>0&wn<=1);
    zeta=zeta(zeta>=0&zeta<=1);

    fig = gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing = "off";
    axes=gca();drawlater(); show_window();
    if new&axes.children<>[] then
        delete(axes.children)
    end
    nc=size(axes.children,"*")
    if nc==0 then
        axes.data_bounds=defaultbounds
        axes.axes_visible="on";
        axes.box="on";
        axes.title.text=msprintf(_("loci with constant damping and constant natural frequency\nin discrete plane"))
        axes.x_label.text=_("Real Axis")
        axes.y_label.text=_("Imaginary Axis")
    end
    axes.fractional_font="on"
    axes.font_size=0.5
    chart_handles=[]

    //                           2                 2
    //roots of                  s  + 2*zeta*wn*s +wn
    //given by : wn*(-zeta+-%i*sqrt(1-sxi*zeta))

    raci=((0:0.05:1)*%pi)'*(-zeta+%i*sqrt(ones(zeta)-zeta.*zeta))
    // continuous --> discrete
    raci=exp(raci);[mr,nr]=size(raci);
    for l=1:nr,
        r=[raci(:,l);conj(raci($:-1:1,l))]
        xpoly(real(r),imag(r))
        ec=gce();
        ec.display_function = "formatZgridDampingTip";
        ec.display_function_data = zeta(l);
        ec.foreground=colors(1),
        ec.line_style=7;
        ec.clip_state="clipgrf";

        xstring(real(raci(mr-10,l)),-imag(raci(mr-10,l))," "+string(zeta(l)),0,0);
        es=gce();
        es.font_foreground=colors(1),
        es.clip_state="clipgrf";
        chart_handles=[glue([es ec]) chart_handles]
    end;

    e_itheta=exp(%i*(%pi/2:0.05:%pi)')
    zw=exp(e_itheta*(wn*%pi));[mz,nz]=size(zw)

    for l=1:nz,
        z=[zw(:,l);zw($:-1:1,l)];
        xpoly(real(z),imag(z))
        ec=gce();
        ec.display_function = "formatZgridFreqTip";
        ec.display_function_data = wn(l);
        ec.foreground=colors(2),
        ec.line_style=7;
        ec.clip_state="clipgrf";
        str=msprintf("%0.3gπ/dt",wn(l))
        xstring(real(zw(1,l)),imag(zw(1,l)),str,0,0);
        es=gce();
        es.font_foreground=colors(2),
        es.clip_state="clipgrf";
        chart_handles=[glue([es ec]) chart_handles]
        xpoly(real(z),-imag(z))
        ec=gce();
        ec.display_function = "formatZgridFreqTip";
        ec.display_function_data = wn(l);
        ec.foreground=colors(2),
        ec.line_style=7;
        ec.clip_state="clipgrf";

        xstring(real(zw(1,l)),-imag(zw(1,l)),str,0,0);
        es=gce();
        es.font_foreground=colors(2),
        es.clip_state="clipgrf";
        chart_handles=[glue([es ec]) chart_handles]
    end;
    chart_handles=glue(chart_handles)
    //reorder axes children to make chart drawn before the  previously
    // drawn curves if any
    for k=1:nc
        swap_handles(axes.children(k),axes.children(k+1))
    end

    if nc==0 then
        axes.data_bounds=defaultbounds
    end

    fig.immediate_drawing = immediate_drawing;

endfunction
