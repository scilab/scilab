// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function fail = setSurfProperty(PropertyName, PropertyValue, Surface, X, Y, Z, C, current_figure, cur_draw_mode)

    fail=0;

    //conversion to lower format
    str = convstr(PropertyName);

    //Property = ['foreground' 'clipping'];

    [PName] = getSurfPropertyName(str,current_figure,cur_draw_mode)

    if (PName==[])
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end

    select PName

        /////////////////////////
    case "colordata"
        /////////////////////////

        if (type(PropertyValue)<>1 | or(size(PropertyValue)==1))
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"setSurfProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        if (size(Z) == size(PropertyValue)) | (size(Z)-1 == size(PropertyValue))
            [tmp1,tmp2,tmp3,CC] = CreateFacetsFromXYZColor(X,Y,Z,PropertyValue);
            Surface.data.color = CC;
        else
            str="surf : incompatible dimensions in input arguments";
            warning(str);
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "colordatamapping"
        /////////////////////////

        if (type(PropertyValue)<>10)
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"setSurfProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        if (PropertyValue=="scaled")
            Surface.cdata_mapping = "scaled";
        elseif (PropertyValue=="direct")
            Surface.cdata_mapping = "direct";
        else
            warning(msprintf(gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"setSurfProperty",2, "scaled", "direct"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "cdatamode"
        /////////////////////////

        //  /////////////////////////
        //case 'clipping'           // Clipping // NO CLIPPING for now with 3d objects
        //  /////////////////////////
        //  if (type(PropertyValue)==10 & (PropertyValue=='on' | PropertyValue=='off'))
        //    Surface.clip_state=PropertyValue;
        //  else
        //    warning("Bad value for property : Clipping")
        //    return;
        //  end


        /////////////////////////
    case "foreground"         // <=> EdgeColor
        /////////////////////////

        if type(PropertyValue) == 10 & PropertyValue=="none"  // NOT DOCUMENTED
            Surface.surface_mode="on";
            Surface.color_mode = 0; // <=> - colormap(1) and not black at all!!
            Surface.mark_foreground = -1; // <=> black
        else
            co = getColorIndFromProp(PropertyValue, current_figure, cur_draw_mode);
            Surface.surface_mode = "on";
            Surface.foreground = co;
            Surface.mark_foreground = co;
        end


        /////////////////////////
    case "facecolor"          // FaceColor
        /////////////////////////

        if (type(PropertyValue) == 10)
            if PropertyValue == "none"
                Surface.surface_mode = "on";
                Surface.color_mode = 0;
                //Surface.mark_foreground = color(ColorVal(index));
                Surface.color_flag = 0;
                return
            elseif PropertyValue == "flat"
                Surface.surface_mode="on";
                Surface.color_flag = 4;
                return
            elseif PropertyValue == "interp"
                Surface.surface_mode="on";
                Surface.color_flag = 3;
                return
            end
        end
        // Inputing a color is not documented in the GlobalProperty page ...
        // Whether the color is specified with a name or with [r,g,b],
        //  not the same Fec3d properties are set...?!
        co = getColorIndFromProp(PropertyValue, current_figure, cur_draw_mode);
        Surface.surface_mode = "on";
        // Strange aternative (equal to the former implementation...):
        if type(PropertyValue)==10
            Surface.color_mode = co;
            //Surface.mark_foreground = co;
            Surface.color_flag = 0;
        else
            Surface.foreground = co;
            Surface.mark_foreground = co;
        end

        /////////////////////////
    case "linestyle"          // LineStyle
        /////////////////////////
        if (type(PropertyValue)==10)
            if (PropertyValue=="--")
                Surface.line_style=2;
                Surface.surface_mode = "on";
            elseif (PropertyValue=="-.")
                Surface.line_style=4;
                Surface.surface_mode = "on";
            elseif (PropertyValue==":")
                Surface.line_style=5;
                Surface.surface_mode = "on";
            elseif (PropertyValue=="-")
                Surface.line_style=1;
                Surface.surface_mode = "on";
            elseif (PropertyValue=="none")
                Surface.surface_mode = "off";
            end
        else
            msg = gettext("%s: Wrong type for input argument #%d: string expected.\n");
            warning(msprintf(msg, "setSurfProperty", 2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "thickness"        // <=> LineWidth
        /////////////////////////
        if (type(PropertyValue)==1)
            Surface.thickness=PropertyValue;
        else
            msg = gettext("%s: Bad value for property ""%s""\n");
            warning(msprintf(msg, "setSurfProperty","thickness"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "markstyle"        // <=> Marker
        /////////////////////////
        if (type(PropertyValue)==10)

            str = convstr(PropertyValue);
            //Marks
            Table = [ "+" "o" "*" "." "x" "square" "diamond" "^" "v" ">" "<" "pentagram" "none"];
            MarksStyleVal=[1 9 10 0 2 11 5 6 7 12 13 14 -99];
            //    MarksSizeVal =[4 3 7  1 3  3 4 3 3  3  3  3 -99];

            opt1=[];

            k=find(part(Table,1:length(str))==str);

            if (k == [])
                msg = gettext("%s: Wrong value for input argument #%d: A marker style expected.\n")
                warning(msprintf(msg, "setSurfProperty", 2));
                PName=[];
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            elseif ( size(k,"*") > 1)
                msg = gettext("%s: Ambiguous MarkStyle value.\n");
                warning(msprintf(msg, "setSurfProperty")); //unreachable case normally
                PName=[];
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

            opt = Table(k);
            for i=1:length(str)
                if part(opt,i) <> part(str,i)
                    break;
                end
            end

            opt1 = [opt1 k];

            str = part(str,i:length(str));

            if (size(opt1,"*") > 1)
                msg = gettext("%s: Wrong value for input argument #%d: A marker style expected.\n");
                warning(msprintf(msg, "setSurfproperty", 2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

            if (opt1 > 0)
                Surface.mark_style = MarksStyleVal(opt1);
                Surface.mark_size  = 6;
                //MarksSizeVal(opt1);
            else
                // 'none' is selected
                Surface.mark_mode="off"
            end

        else
            msg = gettext("%s: Wrong type for input argument #%d: A marker style expected.\n");
            warning(msprintf(msg, "setSurfProperty", 2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "markforeground"        // <=> MarkerEdgeColor
        /////////////////////////

        markmodeON = find(Surface.mark_mode=="on");

        if markmodeON~=[]
            if type(PropertyValue) == 10
                if PropertyValue=="auto"        // NOT DOCUMENTED
                    Surface(markmodeON).mark_foreground =  Surface.foreground;
                    // If Surface may really be a vector of Surface handles, 
                    // this original assignment should not work...
                    return
                elseif PropertyValue=="none"    // NOT DOCUMENTED
                    a = gca();   // pick up the background color of the parent axes
                    Surface(markmodeON).mark_foreground = a.background;
                    return
                end
            end
            co = getColorIndFromProp(PropertyValue, current_figure, cur_draw_mode);
            Surface(markmodeON).mark_foreground = co;
        end


        /////////////////////////
    case "markbackground"        // <=> MarkerFaceColor
        /////////////////////////
        markmodeON = find(Surface.mark_mode=="on");

        if markmodeON ~= []
            if type(PropertyValue) == 10 & or(PropertyValue==["auto" "none"])
                // NOT DOCUMENTED
                a = gca();   // pick up the background color of the parent axes
                Surface(markmodeON).mark_background = a.background;
                return
            end
            co = getColorIndFromProp(PropertyValue, current_figure, cur_draw_mode);
            Surface(markmodeON).mark_background = co;
        end

        /////////////////////////
    case "marksize"        // <=> MarkerSize
        /////////////////////////
        if (type(PropertyValue)==1 & size(PropertyValue,"*")==1)

            markmodeON = find(Surface.mark_mode=="on");

            if markmodeON <> []
                Surface(markmodeON).mark_size = PropertyValue;
            end
        else
            msg = gettext("%s: Wrong type for input argument #%d: A scalar expected.\n");
            warning(msprintf(msg, "setSurfProperty", 2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "visible"        // <=> Visible
        /////////////////////////
        if (type(PropertyValue)==10 & (PropertyValue=="on" | PropertyValue=="off"))
            Surface.visible = PropertyValue;
        else
            msg = gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n");
            warning(msprintf(msg, "setSurfProperty", 2, "on", "off"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "xdata"
        /////////////////////////

        // Already done at the beginning of surf execution.

        /////////////////////////
    case "ydata"
        /////////////////////////

        // Already done at the beginning of surf execution.

        /////////////////////////
    case "zdata"
        /////////////////////////

        // Already done at the beginning of surf execution.
    end

endfunction

function k=getIndexInStringTable(pattern,table)

    str =  convstr(pattern);
    k=find(part(table,1:length(str))==str);

endfunction

function co = getColorIndFromProp(PropertyValue, current_figure, cur_draw_mode)
    c = iscolor(PropertyValue); // "colorName" | "#RRGGBB" | [r,g,b] | indCmap accepted
    co = [];
    if find(c(:,1)~=-1)~=[]
        co = addcolor(c(1,:));
    else    // maybe an abbreviate colorname?
        index = getColorIndex(PropertyValue);
        if index > 0 & index < 10
            Colors = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"]
            co = color(Colors(index));
        else
            msg = gettext("%s: Argument #%d: Wrong color specification.\n")
            warning(msprintf(msg, "setSurfProperty", 2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end
    end
endfunction
