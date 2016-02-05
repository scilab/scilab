// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function h=mtlb_plot(varargin)
    // Emulation function for Matlab plot()

    // Default for Matlab graphics
    mtlbColorOrder=[..
    0    0    1   ;..
    0    0.5  0   ;..
    1    0    0   ;..
    0    0.75 0.75;..
    0.75 0    0.75;..
    0.75 0.75 0   ;..
    0.25 0.25 0.25].*255;

    // red green blue cyan magenta yellow black white
    mtlbcolors=["r"  ,"g"    ,"b"   ,"c"   ,"m"      ,"y"     ,"k"    ,"w"    ]
    scicolors= ["red","green","blue","cyan","magenta","yellow","black","white"]

    mtlbmarkers=["+","o","*",".","x","s","d","^","v",">","<","p","h"]
    scimarkers= [ 1 , 9 , 3 , 0 , 2 , -1, 5 , 6 , 7 , -1, -1, -1, -1]

    mtlblinestyles=["-","--",":","-."]
    scilinestyles= [ 1 , 2  , -1, 5  ]

    // plot(axes_handle,...)
    if type(varargin(1))==9 then
        set("current_axes",varargin(1))
        varargin(1)=null()
    end


    mtlbColor=mtlbColorOrder(1,:)
    colorIndex=1

    // plot(Y)
    if lstsize(varargin)==1 then
        mtlb_plotY(varargin(1))
    elseif lstsize(varargin)==2 then

        // plot(Y,Linespec)
        if type(varargin(2))==10 then
            mtlb_plotY(varargin(1))
            curh=gce()
            curh=curh.children(1)
            mstyl=linespec2sci(varargin(2),curh)
            if ~isempty(mstyl) then
                mtlb_plotY(varargin(1))
                a=gca()
                markplot=a.children(1)
                markplot.children(1).mark_mode="on"
                markplot.children(1).mark_style=mstyl
            end
            if colorIndex<>7 then
                colorIndex=colorIndex+1;
            else
                colorIndex=1;
            end
            mtlbColor=mtlbColorOrder(colorIndex,:);
            // plot(X,Y)
        else
            mtlb_plotXY(varargin(1),varargin(2))
            if colorIndex<>7 then
                colorIndex=colorIndex+1;
            else
                colorIndex=1;
            end
            mtlbColor=mtlbColorOrder(colorIndex,:);
        end
        // plot(X,Y,Linespec)
    elseif lstsize(varargin)==3 & type(varargin(3))==10 then
        mtlb_plotXY(varargin(1),varargin(2))
        curh=gce()
        curh=curh.children(1)
        mstyl=linespec2sci(varargin(3),curh)
        if ~isempty(mstyl) then
            mtlb_plotXY(varargin(1),varargin(2))
            a=gca()
            markplot=a.children(1)
            markplot.children(1).mark_mode="on"
            markplot.children(1).mark_style=mstyl
        end
        if colorIndex<>7 then
            colorIndex=colorIndex+1;
        else
            colorIndex=1;
        end
        mtlbColor=mtlbColorOrder(colorIndex,:);
    else
        k=1
        while k<=lstsize(varargin)
            if k<lstsize(varargin) & type(varargin(k))<>10 then
                mtlb_plotXY(varargin(k),varargin(k+1))
                set(gca(),"auto_clear","off")
                X=varargin(k)
                Y=varargin(k+1)
                lineplot=gce()
                markplot=lineplot
                k=k+2
                if colorIndex<>7 then
                    colorIndex=colorIndex+1;
                else
                    colorIndex=1;
                end
                mtlbColor=mtlbColorOrder(colorIndex,:);
            elseif k<=lstsize(varargin) & type(varargin(k))==10 then
                opt=varargin(k)
                opt=convstr(opt,"l")
                if ~isempty(strindex("linestyle",opt)) then
                    curh=lineplot.children(1)
                    linespec2sci(varargin(k+1),curh)
                    k=k+2
                elseif ~isempty(strindex("linewidth",opt)) then
                    lineplot.children(1).thickness=varargin(k+1)
                    k=k+2
                elseif ~isempty(strindex("marker",opt)) then
                    curh=markplot.children(1)
                    linespec2sci(varargin(k+1),curh)
                    k=k+2
                elseif ~isempty(strindex("markeredgecolor",opt)) then
                    warning(msprintf(gettext("%s: ''%s'' option ignored.\n"),"mtlb_plot","MarkerEdgeColor"));
                    k=k+2
                elseif ~isempty(strindex("markerfacecolor",opt)) then
                    curh=markplot.children(1)
                    linespec2sci(varargin(k+1),curh)
                    k=k+2
                elseif ~isempty(strindex("markersize",opt)) then
                    scimarksize=[8 10 12 14 18 24]
                    markplot.children(1).mark_size=scimarksize(max(find(scimarksize<varargin(k+1))))
                    k=k+2
                else
                    curh=lineplot.children(1)
                    mstyl=linespec2sci(varargin(k),curh)
                    if ~isempty(mstyl) then
                        mtlb_plotXY(X,Y)
                        a=gca()
                        markplot=a.children(1)
                        markplot.children(1).mark_mode="on"
                        markplot.children(1).mark_style=mstyl
                    end
                    k=k+1
                end
            end
        end
    end
    a=gca()
    a.auto_clear="on"
    // Log axis ?
    global mtlb_log_mod
    if ~isempty(mtlb_log_mod)
        a.log_flags = mtlb_log_mod;
    end
    h=a.children
endfunction

function mtlb_plotY(Y)
    if norm(Y,1)==0 then
        plot2d(Y)
    else
        plot2d(real(Y),imag(Y))
    end
    e=gce()
    e.children(1).foreground=color(mtlbColor(1),mtlbColor(2),mtlbColor(3))
    e.children(1).line_style=1
endfunction

function mtlb_plotXY(X,Y)
    plot2d(X,Y)
    e=gce()
    e.children(1).foreground=color(mtlbColor(1),mtlbColor(2),mtlbColor(3))
    e.children(1).line_style=1
endfunction

function mark=linespec2sci(linespec,cur_handle)
    mtlbColor=mtlbColor;
    linep=%F
    // line style
    ind=[]
    for k=1:size(mtlblinestyles,"*")
        found=strindex(linespec,mtlblinestyles(k))
        if ~isempty(found) then
            ind=k
            linep=%T
        end
    end
    styl=scilinestyles(ind)
    if styl>=0 then
        cur_handle.line_style=styl
    elseif ~isempty(ind) then
        warning(msprintf(gettext("%s: Unknown line style found in ''%s'' -> set to 6.\n"),"mtlb_plot",linespec));
        cur_handle.line_style=6
    end

    // color
    ind=[]
    for k=1:size(mtlbcolors,"*")
        found=strindex(linespec,mtlbcolors(k))
        if ~isempty(found) then
            ind=k
            break
        end
    end
    clr=scicolors(ind)
    if ~isempty(clr) then
        cur_handle.foreground=color(clr)
        mtlbColor=name2rgb(clr)
    elseif ~isempty(ind) then
        warning(msprintf(gettext("%s: Unknown color found in ''%s'' -> set to black.\n"),"mtlb_plot",linespec));
        cur_handle.foreground=color("black")
        mtlbColor=name2rgb("black")
    end

    // marker
    ind=[]
    for k=1:size(mtlbmarkers,"*")
        found=strindex(linespec,mtlbmarkers(k))
        if ~isempty(found) then
            ind=k
            break
        end
    end
    mark=scimarkers(ind)
    if ~linep & mark>=0 & ~isempty(mark) then
        cur_handle.mark_mode="on"
        cur_handle.mark_style=mark
        cur_handle.mark_size=1
    elseif ~linep & mark>=0 & ~isempty(mark) then
        cur_handle.mark_mode="on"
        cur_handle.mark_style=4
        cur_handle.mark_size=1
    elseif ~isempty(ind) then
        mark=4
    end
    mtlbColor=resume(mtlbColor)
endfunction

