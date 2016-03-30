// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid Belahcene
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function  barh(varargin)

    // barh(x,y,width,style,color)
    // Input :
    // x : a scalar or a vector of reals
    // y : a scalar, a vector or a matrix of reals
    // width : a double, the bar width, it's the percentage (0<width<1) of the max width of one bar which is wanted (default: width=0.8)
    // style : a string, 'grouped' or 'stacked' (default: style='grouped')

    if and(size(varargin)<>[1:5]) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "barh", 1, 5));
    end

    styletab=["grouped","stacked"]
    COLORBOOL=%f

    // Default values
    STYLE="grouped"
    WIDTH=0.8
    COLORBOOL=%f

    // Check RHS arguments
    ListArg = varargin;

    // Detect and set the current axes now:
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Axes handle expected.\n"),"barh",1));
            return;
        end
    end
    if size(ListArg) == 4 then
        COLOR=ListArg(4);
        if type(COLOR) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"barh",4));
        end
    end
    if size(ListArg) == 5 then
        STYLE=ListArg(5);
        if type(STYLE) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"barh",5));
        end
    end

    nv = size(ListArg)

    T=[];

    for k=1:nv
        T(k) = type(ListArg(k))
    end

    argdb=find(T==1)
    argstr=find(T==10)

    if size(argdb,"*")<> argdb($) then
        error(msprintf(gettext("%s: Wrong type for input arguments: Matrix expected for %s, %s and %s.\n"),"barh", "x", "y", "width"));
    end

    if size(argstr,"*") <> nv-argdb($) then
        error(msprintf(gettext("%s: Wrong type for input arguments: String expected for %s and %s.\n"),"barh", "color", "style"));
    end

    // Set the double argument : x,y,width
    // barh(y,...)
    if size(argdb,"*")==1
        Y=ListArg(1)
        if or(size(Y)==1) then
            Y=Y(:)
        end
        X=1:size(Y,1)
    end

    if size(argdb,"*")==2
        if size(ListArg(2),"*")==1 then
            // barh(x,y,...)
            if size(ListArg(1),"*")==1 then
                X=ListArg(1)
                Y=ListArg(2)
            else
                //barh(y,width,...)
                WIDTH=ListArg(2)
                Y=ListArg(1)
                if or(size(Y)==1) then
                    Y=Y(:)
                end
                X=1:size(Y,1)
            end
        else
            // barh(x,y,...)
            X=ListArg(1)
            Y=ListArg(2)
            if or(size(X)==1) then
                if size(X,"*")<>1 then // X is a vector
                    if or(size(Y)==1) then // Y is a vector
                        Y=Y(:)
                    end
                    if size(X,"*")<>size(Y,1)
                        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: The number of rows of argument #%d must be equal to the size of argument #%d.\n"),"bar",1, 2, 2, 1));
                    end
                elseif size(Y,1)>1 then
                    error(msprintf(gettext("%s: Wrong size for input arguments #%d: A scalar or a column vector expected.\n"),"bar",2));
                end
            else
                error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar or a vector expected.\n"),"barh",1));
            end
        end
    end

    // barh(x,y,width,...)
    if size(argdb,"*")==3
        X=ListArg(1)
        Y=ListArg(2)
        WIDTH=ListArg(3)
        if size(WIDTH,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"),"barh",3));
        elseif or(size(X)==1) then
            if size(X,"*")<>1 then // X is a vector
                if or(size(Y)==1) then // Y is a vector
                    Y=Y(:)
                end
                if size(X,"*")<>size(Y,1)
                    error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: The number of rows of argument #%d must be equal to the size of argument #%d.\n"),"bar",1, 2, 2, 1));
                end
            elseif size(Y,1)>1 then
                error(msprintf(gettext("%s: Wrong size for input arguments #%d: A scalar or a column vector expected.\n"),"bar",2));
            end
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar or a vector expected.\n"),"barh",1));
        end
    end

    X=X(:)

    // Set the string argument
    for i=1:size(argstr,"*")
        // barh(...,style)
        if or(ListArg(argstr(i))==styletab) then
            STYLE=ListArg(argstr(i))
        else
            COLOR=ListArg(argstr(i))
            COLORBOOL=%t
        end
    end

    // Verify if there are data bounds which are defined before creation the horizontal bars creation, in order to merge the data bounds
    a=gca()
    if size(a.children)<>0 then
        gca_children_empty=%t
        a_data_bounds=a.data_bounds
    else
        gca_children_empty=%f
    end

    //drawlater
    curFig = gcf();
    immediate_drawing = curFig.immediate_drawing;

    if COLORBOOL
        plot(X,Y,COLOR); // plot manages immediate_drawing property itself to avoid flickering
    else
        plot(X,Y); // plot manages immediate_drawing property itself to avoid flickering
    end

    curFig.immediate_drawing = "off";

    barh_number=size(Y,2)

    if size(X,"*")>1 then
        Xtemp=gsort(X,"r","i")
        inter=Xtemp(2)-Xtemp(1)
        for i=2:size(Xtemp,"*")-1
            inter=min(Xtemp(i+1)-Xtemp(i),inter)
        end
        if barh_number>1
            inter=inter*0.9
        end
    else
        inter=1
    end

    wmax=inter/barh_number
    y_shift=zeros(size(X,"*"),1)
    bar_number= size(Y,2)
    e=gce()
    a=gca()
    a.sub_ticks(2) = 0

    for i=bar_number:-1:1

        ei = e.children(i)

        // Perform x_shift
        if modulo(bar_number,2)==0 then
            x_shift=(-i+bar_number/2)*wmax+0.4*wmax
        elseif modulo(bar_number,2)==1 then
            x_shift=(-i+1+floor(bar_number/2))*wmax
        end

        // Perform y_shift
        if i==bar_number then
            y_shift=zeros(size(X,"*"),1)
        else
            y_shift=Y(:,bar_number-i)+y_shift
        end

        // Update axes data bounds
        // case 'grouped'
        if STYLE=="grouped"
            if i <> bar_number then
                ymin=min(a.data_bounds(1,1),min(Y(:,bar_number-i+1)),0)
                xmin=min(a.data_bounds(1,2),min(X)+x_shift-0.4*wmax)
                ymax=max(a.data_bounds(2,1),max(Y(:,bar_number-i+1)),0)
                xmax=max(a.data_bounds(2,2),max(X)+x_shift+0.4*wmax)
            else
                if ~gca_children_empty
                    ymin=min(min(Y(:,bar_number-i+1)),0)
                    xmin=min(X)+x_shift-0.4*wmax
                    ymax=max(max(Y(:,bar_number-i+1)),0)
                    xmax=max(X)+x_shift+0.4*wmax
                else
                    ymin=min(a_data_bounds(1,1),min(Y(:,bar_number-i+1)),0)
                    xmin=min(a_data_bounds(1,2),min(X)+x_shift-0.4*wmax)
                    ymax=max(a_data_bounds(2,1),max(Y(:,bar_number-i+1)),0)
                    xmax=max(a_data_bounds(2,2),max(X)+x_shift+0.4*wmax)
                end
            end
            a.data_bounds=[ymin xmin;ymax xmax]
            ei.x_shift=x_shift*ones(size(X,"*"),1)
        else  // case 'stacked'
            wmax=inter
            if i <> bar_number then
                ymin=min(a.data_bounds(1,1),min(Y(:,bar_number-i+1)+y_shift))
                xmin=min(a.data_bounds(1,2),0,min(X-0.4*wmax))
                ymax=max(a.data_bounds(2,1),max(Y(:,bar_number-i+1)+y_shift))
                xmax=max(a.data_bounds(2,2),0,max(X+0.4*wmax))
            else
                if ~gca_children_empty
                    ymin=min(Y(:,bar_number-i+1)+y_shift)
                    xmin=min(0,min(X-0.4*wmax))
                    ymax=max(Y(:,bar_number-i+1)+y_shift)
                    xmax=max(0,max(X+0.4*wmax))
                else
                    ymin=min(a_data_bounds(1,1),min(Y(:,bar_number-i+1)+y_shift))
                    xmin=min(a_data_bounds(1,2),0,min(X-0.4*wmax))
                    ymax=max(a_data_bounds(2,1),max(Y(:,bar_number-i+1)+y_shift))
                    xmax=max(a_data_bounds(2,2),0,max(X+0.4*wmax))
                end
            end
            a.data_bounds=[ymin xmin; ymax xmax]
            ei.y_shift=y_shift
        end

        a.y_ticks=tlist("ticks",Xtemp,string(Xtemp))
        w=WIDTH*wmax
        ei.bar_width=w
        ei.background=ei.foreground
        ei.polyline_style=7; // bar type
        ei.background=ei.foreground
        ei.foreground = -1; // black by default
        ei.line_mode="off";
    end

    //drawnow
    curFig.immediate_drawing = immediate_drawing;

endfunction
