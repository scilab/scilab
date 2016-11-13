// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid Belahcene
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function  barhomogenize(varargin)
    // barhomogenize(a,style,width)
    // This function homogenizes the style and the widh of all the bars contained in the axes handle a (default is the handle of the current axes)
    //
    // Input :
    // a :  the handle of the axe containing the bares (default: a=gca())
    // style : a string, 'grouped' or 'stacked' (default: style='grouped')
    // width : a double, the bar width, it's the percentage (0<width<1) of the width max of one bar which is wanted (default: width=0.8).

    if and(size(varargin)<>[0:3]) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "barhomogenize", 0, 3));
    end

    // Default values
    a=gca()
    STYLE="grouped"
    WIDTH=0.8
    varlist=varargin
    pos="v"

    if size(varargin)<>0 then
        if or(varlist($)==["h";"v"]) then
            pos=varlist($)
            varlist($)=null()
        end
    end
    // detect and set the handle axes, the style and the width
    if size(varlist) == 1
        if type(varlist(1))==9
            var1=varlist(1)
            if var1.type == "Axes"
                a=var1
            else
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Axes handle expected.\n"),"barhomogenize",1));
            end
            varlist(1).type == "Axes"
            a=varlist(1)
        elseif or(varlist(1) == ["grouped","stacked"])
            STYLE=varlist(1)
        elseif type(varlist(1)) == 1
            WIDTH=varlist(1)
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A handle, a string or a scalar expected.\n"),"barhomogenize", 1));
        end
    elseif size(varlist) == 2 then
        if type(varlist(1))==9  then
            var1=varlist(1)
            if (var1.type == "Axes") & or(varlist(2)==["grouped","stacked"]) then
                a=var1
                STYLE=varlist(2)
            elseif var1.type == "Axes" & type(varlist(2)) == 1 then
                a=var1
                WIDTH=varlist(2)
            else
                error(msprintf(gettext("%s: Wrong type for input argument #%d: string or scalar expected.\n"),"barhomogenize", 2));
            end
        elseif or(varlist(1) == ["grouped","stacked"]) & (type(varlist(2)) == 1)
            STYLE=varlist(1)
            WIDTH=varlist(2)
        else
            error(msprintf(gettext("%s: Wrong type for input arguments #%d and #%d: If argument #%d is a handle or a string, argument #%d must be a string or a scalar.\n"),"barhomogenize", 1, 2, 1, 2));
        end
    elseif size(varlist) == 3
        if type(varlist(1))==9  then
            var1 = varlist(1)
            if var1.type == "Axes" & or(varlist(2)==["grouped","stacked"]) & (type(varlist(3)) == 1)
                a=var1
                STYLE=varlist(2)
                WIDTH=varlist(3)
            end
        end
    end

    if ~isreal(WIDTH)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected for %s.\n"),"barhomogenize", length(varlist), "width"));
    end

    hbarlist=[];

    for i=1:size(a.children,"*")
        hbarlist=searchbarhandle(a.children(i),hbarlist)
    end


    nbarhandle=size(hbarlist,"*")
    if hbarlist<>[] then
        X=hbarlist(1).data(:,1)

        for i=2:nbarhandle
            if or(hbarlist(i).data(:,1) <> X)
                error(msprintf(gettext("%s: The bars must have the same x data.\n"),"barhomogenize"));
            end
        end
    else
        error(msprintf(gettext("%s: Could not find any bar in the current graphic.\n"),"barhomogenize"));
    end

    // Determinate the max width
    if size(X,"*")>1 then
        Xtemp=gsort(X,"r","i")
        inter=Xtemp(2)-Xtemp(1)
        for i=2:size(Xtemp,"*")-1
            inter=min(Xtemp(i+1)-Xtemp(i),inter)
        end
        if nbarhandle>1
            inter=inter*0.9
        end
    else
        inter=1
    end

    // Perform the x_shift
    wmax=inter/nbarhandle


    for i=1:nbarhandle
        if modulo(nbarhandle,2)==0 then
            x_shift=(i-1-nbarhandle/2)*wmax+wmax/2
        elseif modulo(nbarhandle,2)==1 then
            x_shift=(i-1-floor(nbarhandle/2))*wmax
        end

        // Perform the y_shift
        if i==1 then
            y_shift=zeros(size(X,"*"),1)
        else
            y_shift=hbarlist(i-1).data(:,2)+y_shift
        end

        // Update axes data bounds
        if pos=="v"

            if STYLE=="grouped"
                a.data_bounds=[min(a.data_bounds(1,1),min(X)+x_shift-wmax/2) min(a.data_bounds(1,2),0,min(hbarlist(i).data(:,2))); max(a.data_bounds(2,1),max(X)+x_shift+wmax/2) max(a.data_bounds(2,2),0,max(hbarlist(i).data(:,2)))]
                hbarlist(i).x_shift=x_shift*ones(size(X,"*"),1)
                hbarlist(i).y_shift=zeros(size(X,"*"),1)
            else
                wmax=inter
                a.data_bounds=[min(a.data_bounds(1,1),min(X)-wmax/2) min(a.data_bounds(1,2),0,min(y_shift+hbarlist(i).data(:,2))); max(a.data_bounds(2,1),max(X)+wmax/2) max(a.data_bounds(2,2),0,max(y_shift+hbarlist(i).data(:,2)))]
                hbarlist(i).x_shift=zeros(size(X,"*"),1)
                hbarlist(i).y_shift=y_shift
            end
            a.auto_ticks(2) = "on";
            a.x_ticks=tlist("ticks",Xtemp,string(Xtemp))
            hbarlist(i).polyline_style=6;
        elseif pos=="h" then
            if STYLE=="grouped"
                a.data_bounds=[min(a.data_bounds(1,2),min(X)+x_shift-wmax/2) min(a.data_bounds(1,1),0,min(hbarlist(i).data(:,2))); max(a.data_bounds(2,2),max(X)+x_shift+wmax/2) max(a.data_bounds(2,1),0,max(hbarlist(i).data(:,2)))]
                hbarlist(i).x_shift=x_shift*ones(size(X,"*"),1)
                hbarlist(i).y_shift=zeros(size(X,"*"),1)
            else
                wmax=inter
                a.data_bounds=[min(a.data_bounds(1,2),min(X)-wmax/2) min(a.data_bounds(1,1),0,min(y_shift+hbarlist(i).data(:,2))); max(a.data_bounds(2,1),max(y_shift+hbarlist(i).data(:,2))) max(a.data_bounds(2,2),0,max(X+x_shift)+wmax/2)]
                hbarlist(i).x_shift=zeros(size(X,"*"),1)
                hbarlist(i).y_shift=y_shift
            end
            a.auto_ticks(1) = "on";
            a.sub_ticks(2) = 0;
            a.y_ticks=tlist("ticks",Xtemp,string(Xtemp))
            hbarlist(i).polyline_style=7;
        end

        w=WIDTH*wmax
        if hbarlist(i).background == -2
            hbarlist(i).background=hbarlist(i).foreground
        end
        hbarlist(i).bar_width=w
        hbarlist(i).line_mode="off";
    end
endfunction

function  hbarlist=searchbarhandle(h,hbarlist)
    // This function searches all the bares contained in an axes handle
    // output :
    // hbarlist: a list which contains the bars handles
    if h.type=="Compound" then
        for j=1:size(h.children,"*")
            hbarlist=searchbarhandle(h.children(j),hbarlist)
        end
    elseif  h.type == "Polyline" then
        if h.polyline_style == 6 | h.polyline_style == 7 then
            hbarlist = [hbarlist h];
        end
    end
endfunction
