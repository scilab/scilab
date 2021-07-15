// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - 2012 - Juergen Koch <juergen.koch@hs-esslingen.de>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = scatter3d(varargin)

    polyLine = 0;
    [lhs, rhs] = argn(0);
    fname = "scatter3d";
    FS = 0  // For argins shift in error messages, to substract from argin rank after Z
    if isdef("_from_Scatter_") then
        fname = "scatter";
        FS = 1  // FromScatter
    end

    // Example 3D
    // ----------
    if ~rhs
        r = gca().axes_bounds; delete(gca()); xsetech(r);
        z = linspace(0,25,200);
        x = z .* cos(z);
        y = z .* sin(z);
        polyLine = scatter3d(x,y,z,10+z.^1.2,z,"fill","markerEdgeColor","darkblue");
        set(gca(),"rotation_angles",[60,45])
        if lhs > 0
            varargout = list(polyLine)
        end
        return
    end

    // PARSING, CHECKING and INITIALIZING MANDATORY ARGUMENTS
    // ------------------------------------------------------
    // graphic handle
        // nh = 1  if primary call with argin#1 = axes, 0 otherwise
    if type(varargin(1)) == 9 then
        axesHandle = varargin(1);
        if axesHandle.type ~= "Axes"
            msg = _("%s: Argument #%d: Wrong type of graphic handle: ''%s'' expected.\n");
            error(msprintf(msg, fname, 1, "Axes"));
        end
        varargin(1) = null()
        if FS==1
            nh = floor(_from_Scatter_)
        else
            nh = 1
        end
    else
        nh = 0
        axesHandle = gca();
    end
    // Number of input arguments
    if length(varargin) < 3 then    // should not occur from scatter() call
        msg = _("%s: Wrong number of input arguments: At least %d expected.\n")
        error(msprintf(msg, fname, 3+nh))
    end
    // Now we know that at least the 3 next argins are defined
    iarg = nh + 1   // rank of next argin

    // X:
    X = varargin(1);
    if ~or(type(X)==[1 5]) | ~isreal(X,0)
        msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
        error(msprintf(msg, fname, iarg))
    else
        X = full(real(X(:)));
    end
    varargin(1) = null();
    iarg = iarg + 1

    // Y:
    Y = varargin(1);
    if ~or(type(Y)==[1 5]) | ~isreal(Y,0)
        msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
        error(msprintf(msg, fname, iarg))
    else
        if length(Y)~=length(X) then
            msg = _("%s: Arguments #%d and #%d: Same numbers of elements expected.\n")
            error(msprintf(msg, fname, nh+1, iarg));
        end
        Y = full(real(Y(:)));
    end
    varargin(1) = null();
    iarg = iarg + 1

    // Z:
    if ~FS then
        Z = varargin(1);
        if ~or(type(Z)==[1 5]) | ~isreal(Z,0)
            msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
            error(msprintf(msg, fname, iarg))
        else
            if length(Z)~=length(X) then
                msg = _("%s: Arguments #%d and #%d: Same numbers of elements expected.\n")
                error(msprintf(msg, fname, nh+1, iarg));
            end
            Z = full(real(Z(:)));
        end
        iarg = iarg + 1
    else
        Z = []
    end
    varargin(1) = null();

    // Empty case
    if X==[] then
        return
    end

    // List of properties names (lower case) (capital = added)
    // "fill"
    // "marker" "markerstyle" "Markstyle"
    // "markerfacecolor" "markerbackgroundcolor"
    // "markeredgecolor" "markerforeground" "markerbackgroundcolor"
    // "thickness" "linewidth"
    // Markers Text styles (available)
    properties = ["marker" "markerstyle" "markstyle" ..
                  "markerfacecolor" "markerbackground" ..
                  "markeredgecolor" "markerforeground" ..
                  "thickness" "linewidth" "datatips"];
    markersT = ["." "+" "x" "o" "o+" "circle plus" "d" "fd" "filled diamond" ..
    "d+" "diamond plus" "^" "v" ">" "<" "*" "*5" "p" "pentagram" "s" "square"];
    // Markers Numerical codes styles (available)
    markersN = [ 0   1   2   9   3        3         5   4         4 ..
      8       8          6   7   12  13  10  14  14      14      11    11];

    // PARSING, CHECKING and INITIALIZING OPTIONS
    // ------------------------------------------
    // Marks sizes | "fill" | marker_symbol |
    msizes = 36;                // Default markers area
    mcolors = [];
    markers = 9;
    fill = %f;
    smallOnTop = %f;
    step = 1;
    // Parsing step: 1 = msizes       2 = mcolors 3 = "fill"
    //             3.5 = "smallOnTop" 4 = marker  5 = properties

    while length(varargin) > 0 & step < 5
       v = varargin(1)
       if type(varargin(1))==0
           v = []
       end
       select step
       case 1
           if type(v) == 10
                if size(v,"*") == 1
                   v = convstr(v)
                   if v=="fill"
                       [fill, step, iarg] = (%t, 3.5, iarg+1)
                       varargin(1) = null()
                       continue
                   end
                   if v=="smallontop"
                       [fill, step, iarg] = (%t, 4, iarg+1)
                       varargin(1) = null()
                       continue
                   end
                   if or(v==markersT)
                       markers = markersN(find(v==markersT, 1))
                       [step, iarg] = (5, iarg+1)
                       varargin(1) = null()
                       continue
                   end
                   if or(v==properties)
                       step = 5
                       continue
                   end
                   msg = _("%s: Argument #%d: Wrong value. Please check allowed syntaxes and values.\n")
                   error(msprintf(msg, fname, iarg))
               else
                   msg = _("%s: Argument #%d: Wrong value. Please check allowed syntaxes and values.\n")
                   error(msprintf(msg, fname, iarg))
               end

           elseif type(v) == 1
               if v==[]
                   [step, iarg] = (2, iarg+1)
                   varargin(1) = null()
                   continue
               end
               if ~and(isreal(v,0))
                   msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
                   error(msprintf(msg, fname, iarg))
               end
               v = real(v);
               if or(v<0)
                   msg = _("%s: Argument #%d: Must be >= %d.\n")
                   error(msprintf(msg, fname, iarg, 0))
               end
               if length(v) ~= 1 & length(v) ~= length(X)
                   msg = _("%s: Arguments #%d and #%d: Same numbers of elements expected.\n")
                   error(msprintf(msg, fname, nh+1, iarg));
               end
               [msizes, step, iarg] = (v, 2, iarg+1)
               varargin(1) = null()

           else
               msg = _("%s: Argument #%d: Number(s) or text expected.\n")
               error(msprintf(msg, fname, iarg))
           end

       case 2   // mcolors
           if type(v)==1
               if ~isreal(v,0)
                   msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
                   error(msprintf(msg, fname, iarg))
               end
               v = real(v)
               if v==[]
                   [step, iarg] = (3, iarg+1)
                   varargin(1) = null()
                   continue
               end
               if length(v) == length(X)
                    // Whole colormap scaled to data
                    [cMin, cMax] = (min(v), max(v));
                    cmap = gcf().color_map;
                    numColors = size(cmap,1);
                    if ((cMax-cMin)/(cMin+cMax) > %eps) then
                        mcolors = round((numColors-1)*(v - cMin)/(cMax - cMin)) + 1;
                    else
                        mcolors = cMin;
                    end
                   [step, iarg] = (3, iarg+1)
                   varargin(1) = null()
                   continue
                end
           end
           c = iscolor(v)
           if ~or(isnan(c(:,1)))
               if length(c) <> 1 & ~(type(c)==1 & and(size(c)==[1 3])) ..
                   & size(c,1) <> length(X)
                   msg = _("%s: Argument #%d: Wrong size: one or %d colors expected.\n")
                   error(msprintf(msg, fname, iarg, length(X)))
               end
               if size(c,2) > 1
                   mcolors = addcolor(c)
               else
                   mcolors = c
               end
               [step, iarg] = (3, iarg+1)
               varargin(1) = null()
           else
               step = 3
           end

       case 3  // "fill"
           if type(v)==10 & size(v,"*")==1
               v = convstr(v)
               if v=="fill"
                   [fill, iarg] = (%t, iarg+1)
                   varargin(1) = null()
               end
            end
            step = 3.5

       case 3.5  // "smallOnTop"
           if type(v)==10 & size(v,"*")==1
               v = convstr(v)
               if v=="smallontop"
                   [smallOnTop, iarg] = (%t, iarg+1)
                   varargin(1) = null()
               end
            end
            step = 4

       case 4
           // symbol or symbol id or property name
           if size(v,"*") <> 1
               msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
               error(msprintf(msg, fname, iarg))
           end
           if and(type(v) <> [1 10])
               msg = _("%s: Argument #%d: Number or text expected.\n")
               error(msprintf(msg, fname, iarg))
           end
           if type(v)==1
               tmp = find(v==markersN, 1)
               if tmp == []
                   msg = _("%s: Argument #%d: Wrong marker id.\n")
                   error(msprintf(msg, fname, iarg))
               end
               markers = v
               [step, iarg] = (5, iarg+1)
               varargin(1) = null()
           else
               v = convstr(v)
               tmp = find(v==markersT, 1)
               if tmp <> []
                   markers = markersN(tmp)
                   [step, iarg] = (5, iarg+1)
                   varargin(1) = null()
               end
               step = 5
           end

       end  // select
   end  // while

    // <Property, Value> pairs
    // .......................
    thickness = 1;
    mBGcolors = [];
    mFGcolors = [];
    datatips = [];

    while length(varargin) > 0
        v = varargin(1)
        if type(v)~=10
            msg = _("%s: Argument #%d: keyword or property name expected.\n")
            error(msprintf(msg, fname, iarg));
        end
        if size(v,"*") > 1
            msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
            error(msprintf(msg, fname, iarg));
        end
        if ~or(convstr(v)==properties)
            msg = _("%s: Argument #%d: Unknown property name ''%s''.\n");
            error(msprintf(msg, fname, iarg, v));
        end
        v = convstr(v);
        varargin(1) = null()
        if length(varargin)==0
            msg = _("%s: Argument #%d: Value missing for the given property.\n")
            error(msprintf(msg, fname, iarg))
        end
        val = varargin(1);
        iarg = iarg+1;
        if or(v==["marker" "markerstyle" "markstyle"]) then
            if size(val,"*")~=1
                msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
                error(msprintf(msg, fname, iarg))
            end
            if type(val)==10 & ~or(convstr(val)==markersT) | ..
               type(val)==1 & ~or(val==markersN)
                msg = _("%s: Argument #%d: Wrong marker specification.\n")
                error(msprintf(msg, fname, iarg))
            end
            if type(val)==10
                markers = markersN(find(markersT==val));
            else
                markers = val
            end
        elseif or(v==["thickness" "linewidth"]) then
            if size(val,"*")~=1
                msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
                error(msprintf(msg, fname, iarg));
            end
            if type(val)~=1 | ~isreal(val,0) | val<0
                msg = _("%s: Argument #%d: Non-negative integers expected.\n");
                error(msprintf(msg, fname, iarg));
            end
            thickness = val;
        elseif or(v==["markeredgecolor" "markerforeground" ..
                      "markerfacecolor" "markerbackground" ]);
            c = iscolor(val);
            if or(isnan(c(:,1))) then
                msg = _("%s: Argument #%d: Wrong color specification.\n");
                error(msprintf(msg, fname, iarg));
            end
            if size(c,1)==1
                if or(v==["markeredgecolor" "markerforeground" "markforeground"]);
                    mFGcolors = c;
                    if length(c)>1
                        mFGcolors = addcolor(c);
                    end
                else
                    mBGcolors = c;
                    if length(c)>1
                        mBGcolors = addcolor(c);
                    end
                end
            else
                msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
                error(msprintf(msg, fname, iarg));
            end

        elseif v=="datatips"
            if typeof(val)=="implicitlist"
                val = horner(val, length(X))
            elseif type(val)<>1 | ~isreal(val) | ..
                or(val<1|val>length(X)) | or(val <> int(val))
                msg = _("%s: Argument #%d: Integers in %s expected.\n")
                tmp = msprintf("[1,%d]",length(X));
                error(msprintf(msg, fname, iarg, tmp))
            end
            // Indices of points for which a datatip must be created
            datatips = val;
        end
        varargin(1) = null();
        iarg = iarg + 1;
    end
    if mFGcolors==[] then
        if mcolors <> [] then
            mFGcolors = mcolors
        else
            mFGcolors = color("blue");
        end
    end
    if mBGcolors==[] then
        if mcolors <> [] & fill
            mBGcolors = mcolors
        else
            if ~fill
                mBGcolors = 0   // Transparent
            elseif mcolors <> []
                mBGcolors = mcolors
            else
                mBGcolors = color("blue")
            end
        end
    end
    if mcolors == [] then
        mcolors = color("blue");    // Default markers color
    end

    // Option "smallOnTop"
    // -------------------
    if smallOnTop & length(msizes) > 1 then
        [msizes, k] = gsort(msizes,"g");
        X = X(k);
        Y = Y(k);
        if Z <> []
            Z = Z(k);
        end
        if length(thickness) > 1
            thickness = thickness(k);
        end
        if length(markers) > 1
            markers = markers(k);
        end
        if length(mFGcolors) > 1
            mFGcolors = mFGcolors(k);
        end
        if length(mBGcolors) > 1
            mBGcolors = mBGcolors(k);
        end
        if length(datatips) > 1
            [?,kk] = gsort(k,"g","i");
            datatips = kk(datatips);
        end
    end

    // ========
    // PLOTTING
    // ========
    f = gcf();
    old_drawing_mode = f.immediate_drawing;
    f.immediate_drawing = "off";

    if isempty(Z) then
        plot(axesHandle, X,Y);
        currentEntity = gce();
        polyLine = currentEntity.children;
    else
        set("current_axes", axesHandle);
        param3d(X,Y,Z);
        polyLine = gce();
    end

    if polyLine.Type <> "Polyline" then
        msg = _("%s: Argument #%d: Wrong type of graphic handle: ''%s'' expected.\n");
        warning(msprintf(msg, fname, -1, "Polyline"));
    else
        polyLine.line_mode = "off";
        polyLine.mark_mode = "on";
        polyLine.thickness = matrix(thickness, -1, 1);
        polyLine.mark_style = matrix(markers, -1, 1);
        polyLine.mark_size_unit = "point";
        polyLine.mark_foreground = matrix(mFGcolors, 1, -1);
        polyLine.mark_background = matrix(mBGcolors, 1, -1);
        polyLine.mark_size = matrix(ceil(2*sqrt(msizes/%pi)),1,-1);

        if datatips <> []
            polyLine.datatip_display_mode = "mouseover";
            dispSize = length(msizes) > 1
            for i = matrix(datatips,1,-1)
                kdat = datatipCreate(polyLine, i);
                if dispSize
                    kdat.user_data = msizes(i)
                    kdat.text = [msprintf("size: %d",msizes(i)) ;
                                 kdat.text ];
                end
            end
            polyLine.datatips.mark_size = 1;
            // polyLine.datatips.mark_mode = "off";
        end
        if ~isempty(Z) then
            isoview off
            set(gca(),"grid",[1 1 1]);
        end
    end

    f.immediate_drawing = old_drawing_mode;

    if lhs > 0 then
        varargout = list(polyLine)
    end
endfunction
