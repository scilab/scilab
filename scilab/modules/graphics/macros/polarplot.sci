// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2010, 2018, 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function polarplot(theta,rho,style,strf,leg,rect)
    [lhs,rhs]=argn(0)
    if rhs<=0 then
        theta=0:.01:2*%pi;
        rho=sin(2*theta).*cos(2*theta)
        s = gca().axes_bounds;
        delete(gca()); xsetech(s) // clears & keeps the current axes area
        polarplot(theta,rho)
        return
    end
    if size(theta,1)==1 then
        theta=theta(:),
    end
    if size(rho,1)==1 then
        rho=rho(:),
    end
    if size(theta,2)==1 & size(rho,2)>1 then
        theta = theta * ones(rho(1,:));
    end
    rm=max(abs(rho))
    x=rho.*cos(theta)
    y=rho.*sin(theta)

    opts=[]
    isstrf=%f;
    isframeflag=%f;
    isrect=%f;
    if exists("style","local")==1 then
        opts=[opts,"style=style"]
    end
    if exists("strf","local")==1 then
        opts=[opts,"strf=strf"]
        isstrf=%t
    end
    if exists("leg","local")==1 then
        opts=[opts,"leg=leg"]
    end
    if exists("rect","local")==1 then
        opts=[opts,"rect=rect"]
        isrect=%t
    end
    if exists("frameflag","local")==1 then
        opts=[opts,"frameflag=frameflag"]
        isframeflag=%t
    end

    if size(opts,2)<rhs-2 then
        error(msprintf(gettext("%s: Wrong value for input argument: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"polarplot","style","strf","leg","rect","frameflag"));
    end

    // Some default values:
    Amin = 0       // starting angle for the frame
    dA = 360       // span of the angular frame
    nQuadrants = 4 // number of quadrants to be drawn

    xmin=min(x);
    xmax=max(x);
    L=(xmax-xmin)*1.07;
    ymin=min(y);
    ymax=max(y);
    H=(ymax-ymin)*1.07;
    // Angle at which Radial labels will be displayed
    A=round(atan((ymin+ymax)/2,(xmin+xmax)/2)/%pi*180/45)*45;
    dx = 0
    dy = 0  // H & V shifts in string-width and string-height units

    // Case without rect=
    if ~isrect then
        // Determines quadrant(s) to be drawn
        Q=[%T %T %T %T];
        e=rm/500;

        if min(x)<-e then
            xmin=-rm;
        else
            xmin=0; Q([2 3])=%F;
        end

        if max(x)>e then
            xmax= rm;
        else
            xmax=0; Q([1 4])=%F;
        end

        if min(y)<-e then
            ymin=-rm;
        else
            ymin=0; Q([3 4])=%F;
        end

        if max(y)>e then
            ymax= rm;
        else
            ymax=0; Q([1 2])=%F;
        end

        L=(xmax-xmin)*1.1; if L==0, L=2*rm*1.1; end
        H=(ymax-ymin)*1.1; if H==0, H=2*rm*1.1; end
        x0=(xmin+xmax)/2; y0=(ymin+ymax)/2;
        rect=[x0-L/2 y0-H/2 x0+L/2 y0+H/2]

        // Special case: data aligned on the x or y axis
        if Q==[%F %F %F %F],
            if (ymax-ymin)<2*e, // on x axis
                if xmin<-e then
                    Q([2 3])=%T
                end
                if xmax> e  then
                    Q([1 4])=%T
                end
            else  // on y axis
                if ymin<-e  then
                    Q([3 4])=%T
                end
                if ymax> e then
                    Q([1 2])=%T
                end
            end
        end

        n=find(Q);   // id numbers of quadrants to be drawn
        nQuadrants=length(n)
        Amin=(n(1)-1)*90

        select nQuadrants
        case 1,
            dA = 90;
            if n==1
                A = 90
                dx = -0.8
            elseif n==2
                A = 90
                dx = 0.8
            elseif n==3
                A = 270
                dx = 0.8
            else
                A = 270
                dx = -0.8
            end
        case 2
            dA = 180;
            if n(1)==1
                if n(2)==2, //A=90, dx=0.0
                else // [1 4]
                    Amin = -90
                    A = 90
                    dx = -0.9
                end
            elseif n(1)==2  // [2 3]
                A = 90
                dx = 0.9
            else            // [3 4]
                A = 0
                dy = 0.6
            end
        else
            A = 90
            Amin = 0
            dA = 360
         end
        opts=[opts,"rect=rect"]
    end // if ~isrect

    if isstrf& isframeflag then
        error(msprintf(gettext("%s: ''%s'' cannot be used with ''%s''.\n"),"polarplot","frameflag","strf"));
    end
    if ~(isstrf) then
        axesflag=0
        opts=[opts,"axesflag=axesflag"],
    end
    if ~(isstrf|isframeflag) then
        frameflag=4
        opts=[opts,"frameflag=frameflag"],
    end

    initDrawingMode = gcf().immediate_drawing;
    gcf().immediate_drawing = "off";
    execstr("plot2d(x,y,"+strcat(opts,",")+")")
    ax = gca();
    ax.margins = [0.09 0.09 0.12 0.09]

    // Frames color
    fcolor = color("grey60");
    txtColor = color("grey30");

    // Default Datatip function for curves
    a = gca();
    curves = a.children(1).children;
    curves.display_function = "polarplot_datatip_display";
    curves.display_function_data = Amin;    // for theta on [0,360] | [-90,90]

    // CIRCULAR FRAME AT SET OF RADII:
    // ------------------------------
    // Radial values for the frame:
    fmt_in=format(), format("v",9)
    // Tunning for smart values:
    p=floor(log10(abs(rm)));
    m=rm/10^p;
    if m<1.3, dm=0.2
    elseif m<=2, dm=0.3
    elseif m<4, dm=0.5
    else dm=1,
    end
    k=fix(m/dm)
    if m-k*dm < dm/5, k=k-1, end
    R=[(1:k)*dm*10^p ]
    // Tuning for smart 10^ display using LaTeX instead of D+## exponential display
    if abs(p)<4,
        Rtxt=string(R)
        [v,k]=max(length(Rtxt))
        tmp=xstringl(0,0,Rtxt(k))
    else
        if dm<1, dm=dm*10, p=p-1, end
        tmp = string(R/10^p)+"108"
        [v,k] = max(length(tmp))
        tmp = xstringl(0,0,tmp(k))
        Rtxt = "$"+string(R/10^p)+"\:.10^{"+string(p)+"}$";
    end
    w = tmp(3); h = tmp(4);
    format(fmt_in(2),fmt_in(1))  // Restoring entrance format
    R = [ R  rm ]

    // Drawing & labelling the radial frame
    kM=size(R,"*");
    for k=1:kM
        r=R(k)
        xarc(-r,r,2*r,2*r,Amin*64,dA*64)
        e = gce();
        e.line_style = 8
        e.foreground = fcolor;
        if k==kM
            e.line_style=1;  // solid outer arc
        else
            xstring(r*cosd(A)+w*dx, r*sind(A)+h*dy, Rtxt(k))
            e = gce();
            e.clip_state = "off";
            e.text_box_mode = "centered"
            e.text_box = [0 0]
            e.font_foreground = txtColor
        end
    end

    // RADIAL FRAME @ SET OF ANGLES:
    // ----------------------------
    if nQuadrants<3, eA=10, else eA=30; end // adaptative angular sampling
    an=linspace(Amin,Amin+dA,round(dA/eA)+1);
    // avoiding 360 == 0
    if nQuadrants>2, tmp=find(abs(an-360)<eA/10); an(tmp)=[]; end
    // Adjusting H-shifts of angular labels
    tmp=xstringl(0,0,"360");
    w=tmp(3); h=tmp(4);
    d = sqrt(w*w + h*h);
    rL = (rm + d*.4)  // Radius for angular labels
    for k = an  // draws and labels angular rays
        xsegs([0;rm*cosd(k)],[0;rm*sind(k)])
        e = gce();
        e.segs_color = fcolor;
        e.line_style = 7;
        xstring(rL*cosd(k), rL*sind(k), string(k))
        e = gce();
        e.text_box_mode = "centered"
        e.text_box = [0 0]
        e.clip_state = "off";
        e.font_foreground = txtColor
    end

    ax.data_bounds=[rect(1:2);rect(3:4)]
    ax.tight_limits(1:2) = ["on" "on"]

    gcf().immediate_drawing = initDrawingMode;
endfunction
