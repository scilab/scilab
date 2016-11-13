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

function plzr(a,b,c,d)
    //
    // Copyright INRIA
    [lhs,rhs]=argn(0)

    if rhs == 0 then
        s=poly(0,"s");
        n=[1+s, 2+3*s+4*s^2, 5;0, 1-s, s];
        d=[1+3*s, 5-s^3, s+1;1+s, 1+s+s^2, 3*s-1];
        h=syslin("c",n./d);
        plzr(h);
        return;
    end

    select typeof(a)
    case "rational" then
        if rhs<>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1)),
        end
        a=tf2ss(a),
        dt=a.dt;
        [a,b,c,d]=a(2:5)
        if type(d)<>1 then
            error(msprintf(gettext("%s: Wrong value of input argument #%d: Proper system expected.\n"),"plzr",1));
        end
    case "state-space" then
        if rhs<>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1)),
        end
        dt=a(7);
        [a,b,c,d]=a(2:5)
        if type(d)<>1 then
            error(msprintf(gettext("%s: Wrong value of input argument #%d: Proper system expected.\n"),"plzr",1));
        end
    case "constant" then
        if rhs<>4 then
            error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"plzr",4)),
        end
        if type(d)<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),..
            "plzr",4));
        end
        dt=[];
    else
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"plzr",1))
        else
            error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"plzr",1))
        end
    end
    if type(d)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),..
        "plzr",4));
    end

    dr=spec(a)
    [al,be]=tr_zer(a,b,c,d)
    nr=al./be
    ni=imag(nr);nr=real(nr)
    di=imag(dr);dr=real(dr)
    //
    mxx=max([nr;dr;1]*1.1)
    mnx=min([nr;dr;-1]*1.1)
    my=max(abs([ni;di;1])*1.1)

    rect=[mnx, -my, mxx, my];

    wdim=get(gcf(),"axes_size")
    dx=(mxx-mnx)/wdim(1);dy=2*my/wdim(2)
    if dy>dx then
        ax=(dy*wdim(1)-mxx+mnx)/2
        mxx=mxx+ax;mnx=mnx-ax
    elseif dy<dx then
        ay=(dx*wdim(2)-2*my)/2
        my=my+ay
    end
    rect=[mnx, -my, mxx, my];

    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    ax=gca();
    ax.data_bounds=[mnx, -my; mxx, my];
    ax.axes_visible="on";

    legs=[],lhandle=[]
    if size(nr,"*")<>0 then
        xpoly(nr,ni)
        e=gce();e.line_mode="off";e.mark_mode="on";
        e.mark_size_unit="point";e.mark_size=7;e.mark_style=5;
        legs=gettext("Zeros")
        lhandle=[e;lhandle]
    end;
    if size(dr,"*")<>0 then
        xpoly(dr,di)
        e=gce();e.line_mode="off";e.mark_mode="on";
        e.mark_size_unit="point";e.mark_size=7;e.mark_style=2;
        legs=[gettext("Poles");legs]
        lhandle=[e;lhandle]
    end
    if dt == "d" | type(dt) == 1 then
        ax.grid=ones(1,3)*color("gray")
        ax.box = "on";
        xarc(-1,1,2,2,0,360*64)
        xtitle(gettext("Transmission zeros and poles"),gettext("Real axis"), ...
        gettext("Imaginary axis"));
    else
        ax.grid=ones(1,3)*color("gray")
        ax.box = "on";
        ymin=ax.data_bounds(1,2);
        ymax=ax.data_bounds(2,2);
        xsegs([0,0],[ymin,ymax])
        xtitle(gettext("Transmission zeros and poles"),gettext("Real axis"), ...
        gettext("Imaginary axis"));
    end

    if legs<>[] then legend(lhandle,legs,1),end
    fig.immediate_drawing=immediate_drawing;
    show_window();
endfunction
