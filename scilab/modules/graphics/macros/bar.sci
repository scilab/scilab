// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid Belahcene
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  bar(varargin)
    // bar(y)
    // bar(x, y)
    // bar(x, y, width, colors, style)
    // bar(x, y [,width] [,colors] [,style])
    // bar(h, ..)
    //
    // Input :
    // x : a real scalar or a vector
    // y : a real scalar, or a vector or a matrix.
    //     If it's a matrix, size(y,"r")==length(x) and size(y,"c") is the 
    //     number of categories that can either be grouped around each x(i),
    //     or stacked at each x(i)
    // width : a double, the bar width, it's the percentage (0<width<1) of the
    //         width max of one bar which is wanted (default: width=0.8)
    // colors: scalar or vector of colors identifiers. If it's a vector, it must
    //         has as many components as there are categories.
    // style : a string 'grouped' or 'stacked' (default: style='grouped')

    fname = "bar"
    ListArg = varargin;

    if size(varargin)<1 | size(varargin)>6  then
        msg = gettext("%s: Wrong number of input argument(s): %d to %d expected.\n")
        error(msprintf(msg, fname, 1, 6));
    end

    // PARSING INPUT ARGUMENTS
    // =======================
    // Targeted axes
    // -------------
    shift = 0;  // shift for argins positions
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
            shift = 1;
        else
            msg = gettext("%s: Argument #%d: Graphic handle of type ''%s'' expected.\n")
            error(msprintf(msg, fname, 1, "Axes"));
        end
    end
    // Style
    // -----
    STYLE = "grouped"
    if length(ListArg)>0 & type(ListArg($))==10 then
        tmp = stripblanks(convstr(ListArg($)))
        if or(tmp=="stacked" | tmp=="grouped")
            if size(tmp,"*")>1
                msg = gettext("%s: Argument #%d: Scalar (1 element) expected.\n")
                error(msprintf(msg, fname, shift+length(ListArg)));
            end
            STYLE = tmp
            ListArg($) = null()
        end
    end
    // bar([h,] y,..)
    // --------------
    msg = gettext("%s: Argument #%d: Decimal number(s) expected.\n")
    if length(ListArg)>0 & (length(ListArg)==1 | and(type(ListArg(2))~=[1 8]) | ..
        (length(ListArg(2))==1 & ListArg(2)>0 & ListArg(2)<=1))  // is width
        Y = ListArg(1)
        if isvector(Y)
            Y = Y(:)
        end
        X = (1:size(Y,1))'
        ListArg(1) = null()
        ky = shift+1
    else
        // bar([h,] x, y,..)  expected
        // X
        if ListArg==list()
            error(msprintf(msg, fname, shift+1));
        end
        X = ListArg(1)
        ListArg(1) = null()
        // Y
        if ListArg==list()
            error(msprintf(msg, fname, shift+2));
        end
        Y = ListArg(1)
        ListArg(1) = null()
        ky = shift+2
        if X==[]
            X = 1:size(Y,1)
        end
        X = X(:)
        if isvector(X) & isvector(Y)
            Y = Y(:)
        end
    end
    if Y==[] then
        msg = gettext("%s: Argument #%d: Non-empty matrix expected.\n")
        error(msprintf(msg, fname, ky))
    end
    if and(type(X)~=[1 8]) | (type(X)==1 & ~isreal(X)) then
        error(msprintf(msg, fname, shift+1))
    else
        X = double(X)
    end
    if and(type(Y)~=[1 8]) | (type(Y)==1 & ~isreal(Y)) then
        error(msprintf(msg, fname, ky))
    else
        Y = double(Y)
    end
    if size(Y,1)~=length(X) then
        msg = gettext("%s: Arguments #%d and #%d: Incompatible sizes.\n")
        error(msprintf(msg, fname, shift+1, shift+2))
    end
    if (isnan(X) | isinf(X))
        msg = gettext("%s: Argument #%d: Inf and Nan values forbidden.\n")
        error(msprintf(msg, fname, shift+1))
    end
    if STYLE=="stacked"
        if (isnan(Y) | isinf(Y))
            msg = gettext("%s: Argument #%d: Inf and Nan values forbidden.\n")
            error(msprintf(msg, fname, ky))
        end
        if or(Y<0)
            msg = gettext("%s: Argument #%d: Non-negative values expected.\n")
            warning(msprintf(msg, fname, ky))
        end
    end

    // Width
    // -----
    kw = ky
    WIDTH = 0.8
    if length(ListArg)>0 & or(type(ListArg(1))==[1 8]) then
        kw = ky+1   // position of the width arg
        WIDTH = ListArg(1)
        if length(WIDTH)>1
            msg = gettext("%s: Argument #%d: Scalar (1 element) expected.\n")
            error(msprintf(msg, fname, kw))
        end
        if WIDTH==[]
            WIDTH = 0.80
        end
        if ~isreal(WIDTH) | WIDTH<0 | WIDTH>1
            msg = gettext("%s: Argument #%d: Must be in the interval %s.\n")
            error(msprintf(msg, fname, kw, "[0, 1]"))
        end
        ListArg(1) = null();
    end

    // Colors
    // ------
    COLOR = []
    if length(ListArg)>0
        COLOR = ListArg(1)
        if COLOR~=[]
            nColors = size(COLOR,"*")
            msg = gettext("%s: Argument #%d: Wrong color specification.\n")
            if type(COLOR)~=10
                error(msprintf(msg, fname, kw+1));
            end
            c = iscolor(COLOR, "a#")
            if or(c(:,1)==-1)
                error(msprintf(msg, fname, kw+1));
            end
            if nColors~=1 & nColors<size(Y,2) then
                msg = _("%s: Arguments #%d and #%d: Incompatible sizes.\n")
                error(msprintf(msg, fname, ky, kw+1));
            end
            if nColors>1 then
                COLOR = COLOR(1:size(Y,2)) // extra components ignored
            else
                COLOR = emptystr(1,size(Y,2)) + COLOR;  // name or "#RRGGBB"
            end
        end
    end

    // PLOTTING
    // ========
    curFig = gcf()
    immediate_drawing = curFig.immediate_drawing
    wmode = warning("query")
    warning("off")
    if COLOR~=[]
        plot(X,Y, "color",COLOR)
    else
        plot(X,Y)
    end
    warning(wmode)
    curFig.immediate_drawing = "off"

    bar_number=size(Y,2)
    if size(X,"*")>1 then
        Xtemp=gsort(X,"r","i")
        inter=Xtemp(2)-Xtemp(1)
        for i=2:size(Xtemp,"*")-1
            inter=min(Xtemp(i+1)-Xtemp(i),inter)
        end
        if bar_number>1
            if STYLE == "stacked"
                inter = inter * 0.9
            else
                inter = inter * 0.7
            end
        end
    else
        Xtemp=X
        inter=1
    end

    wmax=inter/bar_number
    y_shift=zeros(size(X,"*"),1)
    bar_number= bar_number

    e = gce()
    a = gca()

    a.sub_ticks(1) = 0; // bar (barh => a.sub_ticks(2) = 0;)

    for i=bar_number:-1:1

        ei = e.children(i);

        // Perform x_shift
        if modulo(bar_number,2)==0 then
            x_shift=(-i+bar_number/2)*wmax+wmax/2
        elseif modulo(bar_number,2)==1 then
            x_shift=(-i+1+floor(bar_number/2))*wmax
        end

        // Perform y_shift
        if i==bar_number then
            y_shift=zeros(size(X,"*"),1)
        else
            y_shift=Y(:,bar_number-i)+y_shift
        end

        // Udate the axes data bounds
        if STYLE=="grouped"
            xmin = min(a.data_bounds(1,1),min(X)+x_shift-0.45*wmax)
            ymin = min(a.data_bounds(1,2),0,min(y_shift+Y(:,bar_number-i+1)))
            xmax = max(a.data_bounds(2,1),max(X)+x_shift+0.45*wmax)
            ymax = max(a.data_bounds(2,2),0)
            ei.x_shift = x_shift*ones(size(X,"*"),1)
        else
            wmax = inter
            xmin = min(a.data_bounds(1,1),min(X)-0.45*wmax)
            ymin = min(a.data_bounds(1,2), 0, min(y_shift+Y(:,bar_number-i+1)))
            xmax = max(a.data_bounds(2,1),max(X)+0.45*wmax)
            ymax = max(a.data_bounds(2,2), 0, max(y_shift+Y(:,bar_number-i+1)))
            ei.y_shift = y_shift
        end
        a.data_bounds=[xmin ymin; xmax ymax]

        a.x_ticks=tlist("ticks",Xtemp,string(Xtemp))

        w=WIDTH*wmax

        ei.bar_width=w
        ei.background=ei.foreground
        ei.polyline_style=6; // bar type
        ei.background=ei.foreground
        ei.foreground = -1; // black by default
        ei.line_mode="off";
        ei.mark_foreground = -1; // black by default
    end

    // drawnow
    curFig.immediate_drawing = immediate_drawing;

endfunction
