// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [fail]=setPlotProperty(PropertyName,PropertyValue,Curves,current_figure,cur_draw_mode)

    fail=0;

    //conversion to lower format
    str = convstr(PropertyName);



    //Property = ['foreground' 'clipping'];

    [PName] = getPlotPropertyName(str,current_figure,cur_draw_mode)


    if (PName==[])
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end


    select PName

        /////////////////////////
    case "foreground"         // <=> Color
        /////////////////////////

        if (type(PropertyValue) == 10)

            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"];

            if (index < 10)
                Curves.line_mode="on";
                Curves.foreground = color(ColorVal(index));
                Curves.mark_foreground = color(ColorVal(index));
            else  // 'none' selected
                warning(msprintf(gettext("%s: Wrong value for input argument #%d: A color of the colormap expected.\n"),"setPlotProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])

            if (size(PropertyValue,"*")==3)
                Curves.line_mode="on";
                Curves.foreground = addcolor(PropertyValue);
                Curves.mark_foreground = addcolor(PropertyValue);
            else
                warning(msprintf(gettext("%s: Wrong size for input argument #%d: 3x1 or 1x3 vector expected.\n"),"setPlotProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: 3 elements vector or index in the colormap expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "clipping"           // Clipping
        /////////////////////////
        if (type(PropertyValue)==10 & (PropertyValue=="on" | PropertyValue=="off"))
            Curves.clip_state=PropertyValue;
        else
            warning(msprintf(gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"setPlotProperty",2,"on","off"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "linestyle"          // LineStyle
        /////////////////////////
        if (type(PropertyValue)==10)
            if (PropertyValue=="--")
                Curves.line_style=2;
                Curves.line_mode = "on";
            elseif (PropertyValue=="-.")
                Curves.line_style=4;
                Curves.line_mode = "on";
            elseif (PropertyValue==":")
                Curves.line_style=5;
                Curves.line_mode = "on";
            elseif (PropertyValue=="-")
                Curves.line_style=1;
                Curves.line_mode = "on";
            elseif (PropertyValue=="none")
                Curves.line_mode = "off";
            end
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "thickness"        // <=> LineWidth
        /////////////////////////
        if (type(PropertyValue)==1)
            Curves.thickness=PropertyValue;
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"),"setPlotProperty",2));
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
                warning(msprintf(gettext("%s: Wrong value for input argument #%d: A marker style expected.\n"),"setPlotProperty", 2));
                PName=[];
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            elseif ( size(k,"*") > 1)
                warning(msprintf(gettext("%s: Wrong value for input argument #%d: A marker style expected.\n"),"setPlotProperty", 2)); //unreachable case normally
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
                warning(msprintf(gettext("%s: Wrong value for input argument #%d: A marker style expected.\n"),"setPlotproperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

            if (opt1 > 0)
                Curves.mark_style = MarksStyleVal(opt1);
                Curves.mark_size  = 6;
                //MarksSizeVal(opt1);
            else
                // 'none' is selected
                Curves.mark_mode="off"
            end

        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"setPlotProperty"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "markforeground"        // <=> MarkerEdgeColor
        /////////////////////////
        if (type(PropertyValue)==10)

            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white" "none"]

            markmodeON = find(Curves.mark_mode=="on");

            if index == 10
                // 'none' specified
                a=gca(); // pick up the background color of the parent axes
                if markmodeON <> []
                    Curves(markmodeON).mark_foreground = a.background;
                end
            elseif index == 11
                // 'auto' specified
                if markmodeON <> []
                    Curves(markmodeON).mark_foreground =  Curves.foreground;
                end
            else
                if (index==-1)
                    warning(msprintf(gettext("%s: Wrong value for input argument #%d: A color of the colormap expected.\n"),"setPlotProperty",2));
                    ResetFigureDDM(current_figure, cur_draw_mode);
                    return;
                else
                    if markmodeON <> []
                        Curves(markmodeON).mark_foreground = color(ColorVal(index));
                    end
                end
            end
        elseif (type(PropertyValue)==1)
            if (size(PropertyValue,"*")==3)

                markmodeON = find(Curves.mark_mode=="on");
                if markmodeON <> []
                    Curves(markmodeON).mark_foreground = addcolor(PropertyValue);
                end
            else
                warning(msprintf(gettext("%s: Wrong size for input argument #%d: 3x1 or 1x3 vector expected.\n"),"setPlotProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "markbackground"        // <=> MarkerFaceColor
        /////////////////////////
        if (type(PropertyValue)==10)

            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white" "none"]

            markmodeON = find(Curves.mark_mode=="on");

            if index == 10
                // 'none' specified
                a=gca(); // pick up the background color of the parent axes
                if markmodeON <> []
                    Curves(markmodeON).mark_background = a.background;
                end
            elseif index == 11
                // 'auto' specified
                a=gca();
                if markmodeON <> []
                    Curves(markmodeON).mark_background = a.background;
                end
            else
                if (index==-1)
                    warning(msprintf(gettext("%s: Wrong value for input argument #%d: A color of the colormap expected.\n"),"setPlotProperty",2));
                    ResetFigureDDM(current_figure, cur_draw_mode);
                    return;
                else
                    if markmodeON <> []
                        Curves(markmodeON).mark_background = color(ColorVal(index));
                    end
                end
            end
        elseif (type(PropertyValue)==1)

            if (size(PropertyValue,"*")==3)

                markmodeON = find(Curves.mark_mode=="on");
                if markmodeON <> []
                    Curves(markmodeON).mark_background = addcolor(PropertyValue);
                end
            else
                warning(msprintf(gettext("%s: Wrong size for input argument #%d: 3x1 or 1x3 vector expected.\n"),"setPlotProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end

        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "marksize"        // <=> MarkerSize
        /////////////////////////
        if (type(PropertyValue)==1 & size(PropertyValue,"*")==1)

            markmodeON = find(Curves.mark_mode=="on");

            if markmodeON <> []
                Curves(markmodeON).mark_size = PropertyValue;
            end
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: An integer expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    case "visible"        // <=> Visible
        /////////////////////////
        if (type(PropertyValue)==10 & (PropertyValue=="on" | PropertyValue=="off"))
            Curves.visible = PropertyValue;
        else
            warning(msprintf(gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"setPlotProperty",2,"on","off"));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "xdata"
        /////////////////////////

        // Already done at the beginning of plot execution.

        /////////////////////////
    case "ydata"
        /////////////////////////

        // Already done at the beginning of plot execution.

        /////////////////////////
    case "zdata"        // <=> Zdata is treated after the curve was created
        /////////////////////////

        if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
            warning(msprintf(gettext("%s: Wrong size or type for input argument #%d: A column or row vector expected.\n"),"setPlotProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        else
            PropertyValue = PropertyValue(:); // force

            for j=1:size(Curves,"*")
                if size(Curves(i).data,1) <> size(PropertyValue,"*")
                    str="plot : incompatible dimensions in input arguments";
                    warning(str);
                    ResetFigureDDM(current_figure, cur_draw_mode);
                    return;
                else
                    for jj=1:size(PropertyValue,"*")
                        Curves(j).data(jj,3) = PropertyValue(jj);
                    end
                    a=gca();
                    a.view="3d";
                    a.data_bounds=[a.data_bounds(1,1) a.data_bounds(1,2) min(PropertyValue) ;  a.data_bounds(2,1) a.data_bounds(2,2) max(PropertyValue)];
                    a.view="2d";
                end
            end
        end



    end

endfunction
