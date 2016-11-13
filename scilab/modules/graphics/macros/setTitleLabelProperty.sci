// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// Copyright (C) 2013 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [fail] = setTitleLabelProperty(PropertyName,PropertyValue,titlelabel,current_figure,cur_draw_mode)
    // SETTITLELABELPROPERTY function
    // is used by the functions title, xlabel, ylabel, zlabel
    // This function sets the title (or x_, y_, z_label) properties

    fail=0;

    //conversion to lower format
    str = convstr(PropertyName);

    //Property = ['foreground' 'clipping'];

    [PName] = getTitleLabelPropertyNam(str, current_figure, cur_draw_mode)

    if (PName==[])
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end

    select PName

        /////////////////////////
    case "font_size"       // <=> fontsize
        /////////////////////////

        if (type(PropertyValue)<>1 | size(PropertyValue,"*")<>1)
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        titlelabel.font_size = PropertyValue
        return


        /////////////////////////
    case "font_angle"  // <=> rotation
        /////////////////////////
        if (type(PropertyValue)<>1 | size(PropertyValue,"*")<>1)
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        titlelabel.auto_rotation = "off"
        newPropertyValue = modulo(PropertyValue,360)
        titlelabel.font_angle = 360 - newPropertyValue
        return


        /////////////////////////
    case "font_foreground"         // <=> color or edgecolor
        /////////////////////////

        if (type(PropertyValue) == 10)
            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"]

            if index < 10
                titlelabel.font_foreground = color(ColorVal(index));
            elseif index == 10  // 'none' selected
                titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
            else
                warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])

            if (size(PropertyValue,"*")==3)
                titlelabel.font_foreground = addcolor(PropertyValue);
            else
                titlelabel.font_foreground = PropertyValue;
            end

        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "foreground"        // <=> foregroundcolor
        /////////////////////////

        if (type(PropertyValue) == 10)
            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"]

            if index < 10
                titlelabel.fill_mode = "on"
                titlelabel.foreground = color(ColorVal(index));
            elseif index == 10  // 'none' selected
                titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
            else
                warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])

            if (size(PropertyValue,"*")==3)
                titlelabel.fill_mode = "on"
                titlelabel.foreground = addcolor(PropertyValue);
            else
                titlelabel.fill_mode = "on"
                titlelabel.foreground = PropertyValue;
            end
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "background"        // <=> backgroundcolor
        /////////////////////////

        if (type(PropertyValue) == 10)

            index = getColorIndex(PropertyValue);

            ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"]

            if index < 10
                titlelabel.fill_mode = "on"
                titlelabel.background = color(ColorVal(index));
            elseif index == 10  // 'none' selected
                titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
            else
                warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif (type(PropertyValue) == 1) // we entered plot(x,y,'Color',[R,G,B])

            if (size(PropertyValue,"*")==3)
                titlelabel.fill_mode = "on"
                titlelabel.background = addcolor(PropertyValue);
            else
                titlelabel.fill_mode = "on"
                titlelabel.background = PropertyValue;
            end
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Vector or index in the colormap expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "text"        // <=> string
        /////////////////////////
        if (type(PropertyValue) == 10)
            titlelabel.text = PropertyValue;
            return
        else
            warning(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "position"        // <=> position, à completer: la position par rapport à z n'est pas pris en compte, de plus le vecteur position doit contenir des coordonnées utilisateur (coordonnées selon l'axe)
        /////////////////////////

        if type(PropertyValue) == 1 & size(PropertyValue,"*")<=3
            titlelabel.auto_position = "off";
            titlelabel.position = PropertyValue ;
            return
        else
            warning(msprintf(gettext("%s: Wrong size or type for input argument #%d: A real matrix expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end



        /////////////////////////
    case "visible"           // <=> visible
        /////////////////////////

        if type(PropertyValue) == 10
            if or(PropertyValue == ["off" "of"])
                titlelabel.visible = "off";
                return
            elseif PropertyValue == "on"
                titlelabel.visible = "on";
                return
            else
                warning(_("Incorrect input: ..."));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        else
            warning(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end


        /////////////////////////
    case "font_style"     // <=> fontangle
        /////////////////////////
        fonts = ["courrier" "symbol" "times" "times italic" "times bold" ..
        "times bold italic" "helvetica" "helvetica italic" "helvetica bold"..
        "helvetica bold italic" ]
        if typeof(PropertyValue) == "string"
            PropertyValue = convstr(PropertyValue,"l")
            if or(PropertyValue == fonts)
                titlelabel.font_style = find(fonts==PropertyValue)-1;
            else
                warning(msprintf(_("%s: Wrong value for input argument #%d: At least one ""%s"" expected.\n"),"setTitleLabelProperty",2, "font"));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif typeof(PropertyValue) == "constant" & or(PropertyValue==(0:9))
            titlelabel.font_style = PropertyValue;
        else
            warning(msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string matrix expected.\n"),"setTitleLabelProperty",2));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

    end

endfunction


function k = getIndexInStringTable(pattern, table)

    str =  convstr(pattern);
    k = find(part(table,1:length(str))==str);

endfunction


