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

function nicholschart(modules,args,colors)

    [lhs,rhs]=argn(0);

    l10=log(10);
    ratio=%pi/180;

    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    ax=gca();
    old_data_bounds = ax.data_bounds;
    nc=size(ax.children,"*")
    if nc==0 then
        ax.data_bounds=[-360,-40;0,40];
        ax.axes_visible="on";
        ax.box="on";
        ax.tight_limits="on"
        ax.title.text=_("Amplitude and phase contours of y/(1+y)")
        ax.x_label.text=_("phase(y) (degree)");
        ax.y_label.text=_("magnitude(y) (dB)");
    else
        ax.data_bounds(2,2)=max( ax.data_bounds(2,2),40)
    end
    ax.clip_state="clipgrf"

    phi_min=ax.data_bounds(1,1)
    phi_max=ax.data_bounds(2,1)
    mod_min=ax.data_bounds(1,2)
    mod_max=ax.data_bounds(2,2)

    defaultArgs = [1 2 5 10 20 30 50 70 90 120 140 160 180];
    defaultModules=[mod_min:20:-35 -30 -25 -20 -15 -12 -9 -6  -3 -2 -1 -0.5 -0.25 -0.1 0 0.1 0.25 0.5  1  2.3 4  6  12];


    if exists("modules","local") == 0 | modules == [] then
        modules=defaultModules
    else
        if type(modules)<>1|~isreal(modules) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"nicholschart","modules");
        end
        modules=matrix(modules,1,-1)
    end
    if exists("args","local")==0 | args == [] then
        args=defaultArgs
    else
        if type(args)<>1|~isreal(args) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"nicholschart","args");
        end
        args=matrix(args,1,-1)
    end
    //
    if exists("colors","local")==0 | colors == [] then
        colors=[4 12];
    else
        if type(colors)<>1|~isreal(colors) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","colors");
        end
        if size(colors,"*")==1 then
            colors=colors*ones(1,2)
        end
    end
    // convert args to radian and insure negative
    args = -abs(args) * ratio;

    //initialize handles array for chart entities
    chart_handles=[]

    // Replication bounds
    k1=floor(phi_min/180)
    k2=ceil(phi_max/180)

    //isogain curves: y as fixed gain and varying phase
    //-------------------------------------------------
    if modules<>[] then
        w=[linspace(-%pi,-0.1,100) linspace(-0.1,0,80) ]
        nw=size(w,"*")
        for i = 1:prod(size(modules)),
            att=modules(i);
            y=10^(att/20)*exp(%i*w);
            y(y==1)=[];//remove singular point if any
            rf=y./(1-y);
            [module, phi]=dbphi(rf)
            //use symetry and period to extend the curve on [k1*180 k2*180]
            p=[];m=[];
            S=[];cut=[]
            for k=k1:k2-1
                if pmodulo(k,2)==0 then
                    p=[p cut k*180-phi($:-1:1)]
                    m=[m cut module($:-1:1)]
                    if att>0 then
                        str=msprintf("%.2gdB",att)
                        r=xstringl(0,0,str)
                        xstring(k*180-phi($)-r(3)/2,module($),str,0,0),
                        e=gce();
                        e.font_foreground=colors(1)
                        S=[e S]
                    elseif att==0 then
                        l=find(module>mod_max-r(4),1)
                        if l<>[] then
                            xstring(k*180-phi(l-1),module(l-1),"0dB",0,0),
                            e=gce();
                            e.font_foreground=colors(1)
                            S=[e S]
                        end
                    end
                else
                    p=[p cut ((k+1)*180)+phi]
                    m=[m cut module]
                    if att<0 then
                        str=msprintf("%.2gdB",att)
                        r=xstringl(0,0,str)
                        xstring(p($)-r(3),m($),str,0,0),
                        e=gce();
                        e.font_foreground=colors(1)
                        S=[e S]
                    end
                end
                cut=%nan
            end
            xpoly(p,m)
            e=gce();
            e.foreground=colors(1),
            e.line_style=7;
            e.display_function = "formatNicholsGainTip";
            e.display_function_data = att;

            if size(S,"*")>1 then S=glue(S),end
            chart_handles=[glue([S,e]),chart_handles];
        end;
    end

    //isophase curves: y as fixed phase and varying gain
    //-------------------------------------------------

    if args<>[] then

        eps=10*%eps;
        for teta=args,
            //w = teta produce a 0 gain and consequently a singularity in module
            if teta < -%pi/2 then
                last=teta-eps,
            else
                last=teta+eps,
            end;
            //use logarithmic discretization to have more mesh points near low modules
            w=real(logspace(log10(-last),log10(170*ratio),150))
            w=-w($:-1:1)

            n=prod(size(w));
            module=real(20*log((sin(w)*cos(teta)/sin(teta)-cos(w)))/l10)
            w=w/ratio
            //use symetry and period to extend the curve on [k1*180 k2*180]
            p=[];m=[];
            for k=k1:k2-1
                if pmodulo(k,2)==0 then
                    p=[p %nan k*180-w($:-1:1)]
                    m=[m %nan module($:-1:1)]
                else
                    p=[p %nan ((k+1)*180)+w]
                    m=[m %nan module]
                end
            end
            xpoly(p,m)
            e=gce();
            e.foreground=colors(2);
            e.line_style=7;
            e.display_function = "formatNicholsPhaseTip";
            e.display_function_data = teta * 180 / %pi;
            chart_handles=[e chart_handles]
        end;
    end
    chart_handles=glue(chart_handles)
    //reorder axes children to make chart drawn before the black curves if any
    for k=1:nc
        swap_handles(ax.children(k),ax.children(k+1))
    end

    fig.immediate_drawing=immediate_drawing;

    // reset data_bounds
    if rhs == 0 then
        ax.data_bounds=[-360,-40;0,40];
    else
        ax.data_bounds = old_data_bounds;
    end
endfunction
