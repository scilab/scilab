// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function hstruct = hallchart(modules,args,colors)

    // Data bounds
    // -----------
    dataBounds = [-4 -3 ; 3 3];
    axesAlreadyUsed = %f
    if winsid()<>[] then
        ax = gca();
        axesAlreadyUsed = length(ax.children) <> 0
        if axesAlreadyUsed | or(ax.data_bounds<>gda().data_bounds)
            dataBounds = ax.data_bounds
        else
            ax.data_bounds = dataBounds
        end
    end
    [?,k] = max(abs(dataBounds(3:4)))
    Yextrem = dataBounds(2+k)

    // Check/set modules
    // -----------------
    if ~isdef("modules","l") then
        // Default modules
        // ---------------
        yo = Yextrem
        dBp = hallGetDefaultModules(dataBounds(1), yo, 8)
        dBm = hallGetDefaultModules(dataBounds(2), yo, 8)
        modules = gsort([dBm dBp],"g","i")
    else
        if type(modules)<>1 | ~isreal(modules) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","modules");
        end
        modules = matrix(modules,1,-1)
    end

    // Check/set Phases
    // ----------------
    if ~isdef("args","l") then
        args = [-90 -60 -40 -30 -25 -20 -15 -12 12 15 20 25 30 40 60 90]; //in degree
    else
        if type(args)<>1|~isreal(args) then
            error(msprintf("%s: Wrong type for imput argument ""%s"": real floating point array expected\n"),"hallchart","args");
        end
        args = matrix(args,1,-1)
    end

    // Check/set colors
    // ----------------
    defaultCol = ["pink" "skyblue1"]
    if ~isdef("colors","l") then
        colors = defaultCol
    end
    // Manage "" or 0 input values as default color
    if type(colors)==10 then
        if colors(1)=="", colors(1) = defaultCol(1), end
        if size(colors,"*")>1 & colors(2)=="", colors(2) = defaultCol(2), end
    elseif type(colors)==1 & or(colors>1)
        if colors(1)==0, colors(1) = color(defaultCol(1)), end
        if size(colors,"*")>1 & colors(2)==0, colors(2) = color(defaultCol(2)), end
    end
    c = iscolor(colors);
    if or(isnan(c))
        msg = _("%s: Argument #%d: Wrong color specification.\n")
        error(msprintf(msg, "hallchart", 3))
    end
    if size(c,1)==1 then
        c = [c ; c];    // Same color for both subframes
    else
        c = c(1:2, :);  // selects only the 2 first inputs (no warning)
    end
    if size(c,2)==3
        colors = addcolor(c);
    else
        colors = c
    end

    // =======
    // DRAWING
    // =======
    fig = gcf();
    immediate_drawing = fig.immediate_drawing;
    fig.immediate_drawing = "off";

    ax = gca();
    nc = size(ax.children,"*")
    if ~axesAlreadyUsed
        ax.axes_visible = "on";
        xlabel(_("Real axis"), "fontsize",2);
        ylabel(_("Imaginary axis"), "fontsize",2);
        title(_("Hall chart"), "fontsize",3)
        ax.box="on";
    end
    wh = xstringl(0,0,"28dB")(3:4); // Text vshift, for accurate labels placement

    //iso modules circles. Circles whose center are (-M^2/(M^2-1),0) and
    //radius M/(M^2-1) with M=|H(jw)| and H=G/(1+G)

    M = exp(log(10)*modules/20)
    radius = M ./ (M.*M-ones(M))
    xc = -M .* radius
    yc = 0
    radius = abs(radius)
    //arcs replaced by polylines to be able to use datatips
    //  xarcs([xc-radius;yc+radius;2*radius;2*radius;0*M;360*64*ones(M)])
    //  A=gce()
    //  E=unglue(A);
    w = linspace(0,2*%pi,200)
    c = cos(w)
    s = sin(w)
    chart_handles = []
    [gainLines, gainLabels] = ([],[])
    [phaseLines, phaseLabels] = ([],[])

    // Best diagonal angles for positive and negative gain labels:
    // (we assume xmin<-1, xmax>0, ymin<0, ymax>0. Full adaptative automatic
    //  settings might be improved later).
    anglePositiveGainLabels = atand(Yextrem, -1-dataBounds(1))
    angleNegativeGainLabels = atand(Yextrem, dataBounds(2))
    if  anglePositiveGainLabels * angleNegativeGainLabels >0 ..
        & abs(dataBounds(3))==abs(dataBounds(4)) then
        angleNegativeGainLabels = - angleNegativeGainLabels
    end
    // Plotting
    for i = 1:size(M,"*")
        xpoly(xc(i)+radius(i)*c, yc+radius(i)*s)
        ec = gce();
        ec.foreground = colors(1);
        ec.line_style = 7;
        ec.clip_state = "clipgrf";
        ec.display_function = "formatHallModuleTip";
        ec.display_function_data = modules(i);
        // Looks for the intersection of the circle with the straight line
        // to the furthest corner
        if modules(i) > 0
            t = tand(anglePositiveGainLabels)
            b = t^2
            delta = 4*((b-xc(i))^2 - (1+t^2)*(b + xc(i)^2 - radius(i)^2))
            xs = (-2*(b-xc(i)) - sqrt(delta))/(2*(1+t^2))
            ys = -t*(1+xs)
        else
            t = tand(angleNegativeGainLabels)
            b = 0
            delta = 4*((b-xc(i))^2 - (1+t^2)*(b + xc(i)^2 - radius(i)^2))
            xs = (-2*(b-xc(i)) + sqrt(delta))/(2*(1+t^2))
            ys = t*xs
        end

        xstring(xs+sign(modules(i))*wh(1)/3, ys, msprintf("%gdB\n",modules(i)));
        el = gce();

        el.font_foreground = colors(1);
        el.clip_state = "clipgrf";
        el.text_box = [0 0];
        el.text_box_mode = "centered";
        chart_handles = [glue([el ec]) chart_handles];
        gainLines = [gainLines ec]
        gainLabels = [gainLabels el]
    end

    //iso phase circles. Circles whose center are (-1/2, 1/(2*N)) and
    //radius sqrt(1+N^2)/(2*N) with N=tan(arg(H(-jw)))

    N = tan(args/180*%pi);
    radius = sqrt(1+N.^2)./(2*N);
    xc = -1/2;
    yc = 1 ./ (2*N);
    //  xarcs([xc-radius;yc+radius;2*radius;2*radius;0*N;360*64*ones(N)])
    //  E=unglue(gce());
    for i = 1:size(N,"*")
        xpoly(xc+radius(i)*c, yc(i)+radius(i)*s);
        ec = gce();
        ec.foreground = colors(2);
        ec.line_style = 7;
        ec.clip_state = "clipgrf";
        ec.display_function = "formatHallPhaseTip";
        ec.display_function_data = args(i);
        xstring(xc, yc(i)+radius(i)+wh(2)/4, msprintf("%g°", args(i)));
        el = gce();
        el.text_box = [0 0];
        el.text_box_mode = "centered";
        el.font_foreground = colors(2);
        el.clip_state = "clipgrf";
        chart_handles = [glue([el ec]) chart_handles];
        phaseLines = [phaseLines ec]
        phaseLabels = [phaseLabels el]
    end
    chart_handles = glue(chart_handles)
    //reorder axes children to make chart drawn before the previously
    // drawn curves if any
    for k = 1:nc
        swap_handles(ax.children(k), ax.children(k+1))
    end
    ax.data_bounds = dataBounds
    fig.immediate_drawing = immediate_drawing
    hstruct = struct("gainLines" ,gainLines, "gainLabels"  ,gainLabels, ..
                     "phaseLines",phaseLines, "phaseLabels",phaseLabels)
endfunction
// ---------------------------------------------------------------------------
function gains = hallGetDefaultModules(xo, yo, n)
    // xo: abscissa of the left/right axes bound
    // yo: ordinates of the furthest bottom|top axes bound
    // n: number of default modules

    a = (xo^2 + yo^2)
    //      Search the circle of the (smallest) one passing
    //      to the furthest left corner (xo,yo)
    M2 = a / (1 + a + 2*xo)
    Rmax = sqrt(M2) / abs(M2-1)
    R = linspace(0, Rmax, n)(2:$);
    //      Search the circle whose left/right side touches the left/right axes bound
    Medge = xo / (1+xo)
    //      Corresponding dB
    dBedge = 20*log10(Medge)

    if xo < 0 then
        // Positive gains:
        //      Generating corresponding dB
        M = (1 + sqrt(1+4*R.^2)) ./ R / 2
        dB = 20*log10(M)
        //      If dBleft > dB(1) => we add it as small circle
        if dBedge > dB(1)
            dB = [dB(1)^2/dB(2) dB]
        end
        Round = ceil
    else
        // Negative gains:
        //      Generating corresponding dB
        M = (-1 + sqrt(1+4*R.^2)) ./ R / 2
        dB = 20*log10(M)
        //      If dBright < dB(1) => we add it as small circle
        if dBedge < dB(1)
            dB = [dB(1)^2/dB(2) dB]
        end
        Round = floor
    end

    // Special rounding of dB values
    d = abs(dB(1:$-1) - dB(2:$))
    d($+1) = d($)^2/d($-1)
    f = 10^floor(log10(d))
    // At last, we have them:
    gains = Round(dB ./ f).*f
endfunction
