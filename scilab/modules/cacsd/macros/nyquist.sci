// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1984-2011 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function nyquist(varargin)
    // Nyquist plot
    //!
    rhs=size(varargin);

    if rhs == 0 then
        //Hall chart as a grid for nyquist
        s=poly(0,"s");
        Plant=syslin("c",16000/((s+1)*(s+10)*(s+100)));
        //two degree of freedom PID
        tau=0.2;xsi=1.2;
        PID=syslin("c",(1/(2*xsi*tau*s))*(1+2*xsi*tau*s+tau^2*s^2));
        nyquist([Plant;Plant*PID],0.5,100,["Plant";"Plant and PID corrector"]);
        hallchart(colors=color("light gray")*[1 1])
        //move the caption in the lower right corner
        ax=gca();Leg=ax.children(1);
        Leg.legend_location="in_upper_left";
        return;
    end

    symmetry=%t
    if type(varargin(rhs))==4 then //symmetrization flag
        symmetry=varargin(rhs)
        rhs=rhs-1
    end
    if type(varargin(rhs))==10 then
        comments=varargin(rhs);
        rhs=rhs-1;
    else
        comments=[];
    end
    fname="nyquist";//for error messages
    fmax=[];
    if or(typeof(varargin(1))==["state-space" "rational"]) then
        //sys,fmin,fmax [,pas] or sys,frq
        refdim=1; //for error message
        sltyp=varargin(1).dt;
        if rhs==1 then
            [frq,repf,splitf]=repfreq(varargin(1),1d-3,1d3);
        elseif rhs==2 then //sys,frq
            if size(varargin(2),2)<2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),fname,2,1))
            end
            [frq,repf]=repfreq(varargin(1:rhs));
        elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
            [frq,repf,splitf]=repfreq(varargin(1:rhs));
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,1,5))
        end
    elseif  type(varargin(1))==1 then
        //frq,db,phi [,comments] or frq, repf [,comments]
        refdim=2;
        sltyp="x";
        splitf=[];
        splitf=1;
        select rhs
        case 2 then //frq,repf
            frq=varargin(1);
            repf=varargin(2);
            if size(frq,2)<2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),fname,1,1))
            end
            if size(frq,2)<>size(varargin(2),2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),fname,1,2))
            end

        case 3 then  //frq,db,phi
            frq=varargin(1);
            if size(frq,2)<>size(varargin(2),2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),fname,1,2));
            end
            if size(frq,2)<>size(varargin(3),2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),fname,1,3));
            end
            repf=exp(log(10)*varargin(2)/20 + %pi*%i/180*varargin(3));

        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1));
    end;
    if size(frq,1)==1 then
        ilf=0;
    else
        ilf=1;
    end

    [mn,n]=size(repf);
    if and(size(comments,"*")<>[0 mn]) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),fname,refdim,rhs+1));
    end
    //

    repi=imag(repf);
    repf=real(repf);

    // computing bounds of graphic window
    mnx=min(-1,min(repf));// to make the critical point visible
    mxx=max(-1,max(repf));

    if symmetry then
        mxy=max(0,max(abs(repi)));
        mny=min(0,-mxy);
    else
        mxy=max(0,max(repi));
        mny=min(0,min(repi));
    end
    dx=(mxx-mnx)/30;
    dy=(mxy-mny)/30;
    rect=[mnx-dx,mny-dy;mxx+dx,mxy+dy];

    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    ax=gca();
    if ax.children==[] then
        ax.data_bounds=rect;
        ax.axes_visible="on";
        ax.grid=color("lightgrey")*ones(1,3)
        ax.title.text=_("Nyquist plot");
        if sltyp=="c" then
            ax.x_label.text=_("Re(h(2iπf))");
            ax.y_label.text=_("Im(h(2iπf))");
        elseif sltyp=="x" then
            ax.x_label.text=_("Re");
            ax.y_label.text=_("Im");
        else
            ax.x_label.text=_("Re(h(exp(2iπf*dt)))");
            ax.y_label.text=_("Im(h(exp(2iπf*dt)))");
        end
    else
        rect= ax.data_bounds
        mnx=rect(1,1);
        mxx=rect(2,1)
        mny=rect(1,2)
        mxy=rect(2,2)
    end

    // drawing the curves
    splitf($+1)=n+1;

    ksplit=1;sel=splitf(ksplit):splitf(ksplit+1)-1;
    R=[repf(:,sel)];  I=[repi(:,sel)];
    F=frq(:,sel);
    for ksplit=2:size(splitf,"*")-1
        sel=splitf(ksplit):splitf(ksplit+1)-1;
        R=[R %nan(ones(mn,1)) repf(:,sel)];
        I=[I %nan(ones(mn,1)) repi(:,sel)];
        F=[F %nan(ones(size(frq,1),1)) frq(:,sel)];
    end
    Curves=[]

    kf=1
    if symmetry then
        for k=1:mn
            xpoly([R(k,:) R(k,$:-1:1)],[I(k,:) -I(k,$:-1:1)]);
            e=gce();e.foreground=k;
            e.display_function = "formatNyquistTip";
            e.display_function_data = [F(kf,:) -1*F(kf,$:-1:1)];
            Curves=[Curves,e];
            kf=kf+ilf;
        end
    else
        for k=1:mn
            xpoly(R(k,:),I(k,:));
            e=gce();e.foreground=k;
            e.display_function = "formatNyquistTip";
            e.display_function_data = F(kf,:);
            Curves=[Curves,e];
            kf=kf+ilf;
        end
    end
    clear R I

    kk=1;p0=[repf(:,kk) repi(:,kk)];ks=1;d=0;
    dx=rect(2,1)-rect(1,1);
    dy=rect(2,2)-rect(1,2);
    dx2=dx^2;
    dy2=dy^2;

    // collect significant frequencies along the curve
    //-------------------------------------------------------
    Ic=min(cumsum(sqrt((diff(repf,1,"c").^2)/dx2+ (diff(repi,1,"c").^2)/dy2),2),"r");
    kk=1;
    L=0;
    DIc=0.2;
    while %t
        if isempty(Ic) then
            break
        end
        ksup=find(Ic-L>DIc);
        if ksup==[] then break,end
        kk1=min(ksup);
        L=Ic(kk1);
        Ic(1:kk1)=[];
        kk=kk+kk1;

        if min(abs(frq(:,ks($))-frq(:,kk))./abs(frq(:,kk)))>0.001 then
            if min(sqrt(((repf(:,ks)-repf(:,kk)*ones(ks)).^2)/dx2+..
                ((repi(:,ks)-repi(:,kk)*ones(ks)).^2)/dy2)) >DIc then
                ks=[ks kk];
                d=0;
            end
        end
    end
    if ks($)~=n then
        if min(((repf(:,ks(1))-repf(:,n)).^2)/dx2+((repi(:,ks(1))-repi(:,n)).^2)/dy2)>0.01 then
            ks=[ks n];
        end
    end
    // display of parametrization (frequencies along the curve)
    //-------------------------------------------------------
    kf=1
    if ks($)<size(repf,2) then last=$;else last=$-1;end
    for k=1:mn,
        L=[];
        for kks=ks
            xstring(repf(k,kks),repi(k,kks),msprintf("%-0.3g",frq(kf,kks)),0);
            e=gce();e.font_foreground=k;
            L=[e L];
            if symmetry&(abs(repi(k,kks))>mxy/20) then //not to overlap labels
                xstring(repf(k,kks),-repi(k,kks),msprintf("%-0.3g",-frq(kf,kks)),0);
                e=gce();e.font_foreground=k;
                L=[e L];
            end
        end
        L=glue(L);
        A=[];

        if size(ks,"*")>1 then
            dr=repf(k,ks(1:last)+1)-repf(k,ks(1:last));
            di=repi(k,ks(1:last)+1)-repi(k,ks(1:last));
            dd=1500*sqrt((dr/dx).^2+(di/dy).^2);
            dr=dr./dd;
            di=di./dd;
            // we should use xarrows or xsegs here.
            // However their displayed arrow size depends
            // on the data bounds and we want to avoid this
            if symmetry then
                xx=[repf(k,ks(1:last))         repf(k,ks(last:-1:1))+dr($:-1:1) ;
                repf(k,ks(1:last))+dr      repf(k,ks(last:-1:1))]
                yy=[repi(k,ks(1:last))        -repi(k,ks(last:-1:1))-di($:-1:1) ;
                repi(k,ks(1:last))+di     -repi(k,ks(last:-1:1))]
            else
                xx=[repf(k,ks(1:last))    ;
                repf(k,ks(1:last))+dr]
                yy=[repi(k,ks(1:last));
                repi(k,ks(1:last))+di]
            end
            xpolys(xx,yy)
            //xarrows([repf(k,ks(1:last));repf(k,ks(1:last))+dr],..
            //    [repi(k,ks(1:last));repi(k,ks(1:last))+di],1.5)
            A=gce();
            A.children.arrow_size_factor = 1.5;
            A.children.polyline_style = 4;
            A.children.foreground=k;
        end

        kf=kf+ilf;
        glue([Curves(k) glue([L A])]);

    end;

    if comments<>[] then
        legend(Curves, comments);
    end
    fig.immediate_drawing=immediate_drawing;
endfunction
