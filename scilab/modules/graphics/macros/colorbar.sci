// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
// Copyright (C) Serge Steer (adaptation to new graphic system)
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function colorbar(umin, umax, colminmax, fmt)

    //  PURPOSE
    //     Draw a colorbar for a plot3d, fec, Sgrayplot, etc...
    //
    //  PARAMETERS
    //     umin : min value of the plot
    //     umax : max value of the plot
    //     colminmax : (optional) a vector with 2 integer components
    //                 the first is the color number (of the current
    //                 colormap) associated with umin
    //                 the second the max color number ....
    //                 default : [1 nb_colors] where nb_colors is
    //                 the number of colors of the current colormap.
    //                 May be useful to deal with a part of the colormap
    //                 (for instance using fec or plot3d)
    //     fmt : optional, a C format to display colorbar graduations
    //
    //  CAUTION
    //     this function may be used BEFORE a plot3d, fec, Sgrayplot, ...
    //     It is important because this function set and change the
    //     frame for the plot. This way the colorbar is not part of
    //     the "associated" plot and so is not modified by a zoom or
    //     a rotation of the plot.
    //
    //  EXAMPLES
    //     see the help page
    //
    //  HISTORY
    // 2017 : http://bugzilla.scilab.org/14711 : in uicontrol frame
    // 2018 : http://bugzilla.scilab.org/15638 : unequal color spans
    //        http://bugzilla.scilab.org/15805 : poor ticking
    //        http://bugzilla.scilab.org/15806 : syntaxes with default umin, umax..

    // Check number of input argument
    [lhs, rhs] = argn();
    if rhs > 4 then
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "colorbar", 0, 4));
    end

    // TYPE OF THE ASSOCIATED PLOT
    // ===========================
    Type = []
    f = gcf();
    nColorsCM = size(f.color_map,1);
    for h = gca().children'
        if or(h.type==["Matplot" "Fec" "Fac3d" "Plot3d" "Grayplot"])
            Type = h.type
            break
        end
        for g = h.children'
            if or(g.type==["Matplot" "Fec" "Fac3d" "Plot3d" "Grayplot"])
                Type = g.type
                h = g
                break
            end
        end
        if Type~=[] then
            break
        end
    end
    if Type=="Matplot" & h.image_type~="index" then
        Type = []
    //elseif Type=="Fac3d" & h.cdata_mapping=="direct"
    //    Type = "Matplot"      // done later
    end

    // PARSING INPUT ARGUMENTS
    // =======================
    // colminmax
    if isdef("colminmax","l") & type(colminmax)~=0 & colminmax~=[] & colminmax(1)~=-1
        msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
        if type(colminmax)~=1 | ~isreal(colminmax)
            error(msprintf(msg, "colorbar", 3))
        end
        if length(colminmax)~=2
            msg = _("%s: Argument #%d: Vector with %d elements expected.\n")
            error(msprintf(msg, "colorbar", 3, 2))
        end
        colminmax = gsort(colminmax,"g","i");
        if and(colminmax>=0 & colminmax<=1) // fractions of the whole colormap range
            colminmax = round(1 + colminmax*(nColorsCM-1))
        end
        colminmax = [max(colminmax(1),1) min(colminmax(2), nColorsCM)];
    elseif ~isdef("colminmax","l") | type(colminmax)==0 | colminmax==[]
        if ~isdef("umin","l") | type(umin)==0 | umin==[] then
            colminmax = -1
        else
            colminmax = [1, nColorsCM]
        end
    else
        colminmax = -1
    end

    // Default umin, umax, colminmax
    if Type=="Fec"
        u = h.data(:,3);
    elseif Type=="Plot3d"
        u = h.data.z
    elseif Type=="Fac3d"
        u = h.data.z;
        c = h.data.color;
        colorsAreZ = ~isvector(c)
        //if colorsAreZ
        //    // c = U*a+b
        //    c2 = c-mean(c);
        //    u2 = u-mean(u);
        //    k = u~=0;
        //    colorsAreZ = colorsAreZ & stdev(c(k)./u(k))==0 // Improvement to explore
        //end
        if colorsAreZ
            select h.color_flag
            case 0
                u = []
            case 1
                u = mean(u,"r")
            case 2
                u = mean(u,"r")
            case 3  // keep u as is
            case 4
                u = u(1,:)
            end
        else
            u = []  // no Z-colors mapping possible
        end

    elseif Type=="Grayplot"
        u = h.data.z
        u = (u(1:$-1,1:$-1)+u(2:$,1:$-1)+u(2:$,2:$)+u(1:$-1,2:$))/4;
        if h.data_mapping=="direct"
            Type = "Matplot"
        end
    elseif Type=="Matplot"
        u = h.data
    else
        u = []
    end
    k = ~isinf(u) & ~isnan(u)
    uminmax = [min(u(k)) max(u(k))]
    clear k

    // umin
    if ~isdef("umin","l") | type(umin)==0 | umin==[] then
        if u~=[]
            if colminmax~=[] & (length(colminmax)>1 | colminmax~=-1)
                if Type=="Matplot"
                    umin = colminmax(1)
                else
                    if argn(2)<2
                        c = colminmax // raw bounds (not integers)
                        nc = max(1,floor(colminmax(1)))
                        // recomputing umin matching the rounded colminmax(1)
                        umin = uminmax(1) + (uminmax(2)-uminmax(1)) * ..
                                        (colminmax(1)-nc)/(c(2)-c(1))
                    else
                        umin = uminmax(1) + (uminmax(2)-uminmax(1)) * ..
                                        (colminmax(1)-0)/nColorsCM
                    end
                end
            else
                umin = uminmax(1)
            end
        else
            msg =_("%s: Argument #%d: Can''t retrieve a default value: Decimal number expected.\n")
            error(msprintf(msg, "colorbar", 1))
        end
    else
        msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
        if type(umin)~=1 | ~isreal(umin)
            error(msprintf(msg, "colorbar", 1))
        end
        if length(umin)>1
            msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
            error(msprintf(msg, "colorbar", 1))
        end
        if umin==-%inf
            umin = uminmax(1)  // umin=-%inf means umin=min(u)
        end
    end

    // umax
    if ~isdef("umax","l") | type(umax)==0 | umax==[] then
        if u~=[]
            if colminmax~=[] & colminmax~=-1
                if Type=="Matplot"
                    umax = colminmax(2)
                else
                    if argn(2)<2
                        c = colminmax // raw bounds (not integers)
                        nc = min(nColorsCM,ceil(colminmax(2)))
                        // recomputing umax matching the rounded colminmax(1)
                        umax = uminmax(2) + (uminmax(2)-uminmax(1)) * ..
                                        (nc-c(2))/(c(2)-c(1))
                    else
                        umax = uminmax(1) + (uminmax(2)-uminmax(1)) * ..
                                            (colminmax(2)-0)/nColorsCM
                    end
                end
            else
                umax = uminmax(2)
            end
        else
            msg =_("%s: Argument #%d: Can''t retrieve a default value: Decimal number expected.\n")
            error(msprintf(msg, "colorbar", 1))
        end
    else
        msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
        if type(umax)~=1 | ~isreal(umax)
            error(msprintf(msg, "colorbar", 2))
        end
        if length(umax)>1
            msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
            error(msprintf(msg, "colorbar", 2))
        end
        if umax==%inf
            umax = uminmax(2)  // umax=%inf means umax=max(u)
        end
    end

    // colminmax
    if colminmax(1)==-1  // => relative color range matches relative u range
        if Type=="Matplot"
            colminmax = [umin umax]
        else
            colminmax = 1 + (nColorsCM-1) * ..
                       ([umin umax]-uminmax(1)) / (uminmax(2)-uminmax(1))
        end
    end
    if Type~="Matplot"
        colminmax = [max(1,round(colminmax(1))) min(nColorsCM,round(colminmax(2)))]
    end

    // fmt
    if isdef("fmt","l") then
        if type(fmt)<>10 | size(fmt,"*")<>1 then
            msg = gettext("%s: Wrong type for input argument #%d: %s expected.\n")
            error(msprintf(msg, "colorbar", argn(2), "string (containing a C format)"));
        end
    else
        fmt = ""
    end

    // DRAWING
    // =======
    //defer the drawing to avoid binking
    idMem = f.immediate_drawing;
    f.immediate_drawing = "off";
    // get current axes and properties
    a = gca();
    fg_color=a.foreground
    wr=a.axes_bounds; //get the rectangle of the current axes

    // modify the orginal axes to let space for the colorbar
    a_pl=a;
    a_pl.axes_bounds=[wr(1) , wr(2) , 0.85*wr(3) , wr(4)]


    // create a new axes for the colorbar et set its properties
    a_cb = newaxes(a.parent);
    a_cb.axes_bounds=[wr(1)+0.83*wr(3) , wr(2)+wr(4)*0.2 , 0.2*wr(3) , wr(4)*0.6];
    a_cb.foreground = a.foreground;
    a_cb.background = f.background;
    a_cb.axes_visible = "on";
    a_cb.y_location   = "right";
    a_cb.tight_limits = "on";

    //It is not possible to set no ticks for x (should be fixed)
    a_cb.x_ticks=tlist(["ticks","locations","labels"],-1,"");
    a_cb.auto_ticks = ["off","on","off"];
    a_cb.ticks_format(2) = fmt;
    a_cb.box = "on";
    a_cb.margins=[0 0.75 0 0];

    //draw the colorbar
    Matplot((colminmax(2):-1:colminmax(1))')
    a_cb.y_location = "right";
    a_cb.tight_limits = "on";
    if Type~="Matplot" then
        du = (umax-umin)
        gce().rect = [0.5 umin 1.5 umax];
        a_cb.data_bounds  = [0.5, 1.5, umin-du/500, umax+du/500];
    else
        s = ((umax-umin)==(colminmax(2)-colminmax(1)))*0.5
        gce().rect = [0.5 umin-s 1.5 umax+s];
        a_cb.data_bounds  = [0.5 1.5 umin-s umax+s];
    end

    //reset the initial values
    sca(a_pl) //current axes

    // Restoring input drawing mode
    f.immediate_drawing = idMem;
endfunction
