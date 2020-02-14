// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2013, 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function TitleLabel(varargin)
    // TITLELABEL function
    // This internal is used by the functions title, xlabel, ylabel, zlabel

    [lhs,rhs]=argn(0);

    ListArg = varargin;
    titlelabel = ListArg(1);
    fname = strsubst(titlelabel,"_","");// for error messages
    ListArg(1) = null();  // remove this parameter from the list
    arginPos = 0;                       // to compute argin # for error messages

    //detect and set the current axes now:
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
            arginPos = 1;
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Axes handle expected.\n"), fname, 1));
            return;
        end
    end

    monaxe = gca();

    nv = size(ListArg);
    if  modulo(nv,2) == 1 then
        // detect and set the string now:
        if  type(ListArg(1))== 10
            st = ListArg(1);
            execstr("monaxe."+ titlelabel +".text"+"=st")
            ListArg(1) = null();
            nv=nv-1
            arginPos = arginPos + 1;
        else
            msg = gettext("%s: Wrong size or type for input argument #%d: string expected.\n")
            error(msprintf(msg, fname, arginPos+1));
        end
    end

    T = [];

    for k=1:nv
        T(k,1) = type(ListArg(k))
    end

    given_data = 0;
    P1 = 0;

    for i=1:nv
        if T(i) == 1
            given_data = given_data +1;
        else
            P1 = i;  // Position of the first PropertyName field
            break;
        end
    end

    // delay the drawing commands
    // smart drawlater
    current_figure = gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    ///////////////////////////////////
    //Global Property treatment      //
    //PropertyName and PropertyValue //
    ///////////////////////////////////

    // P1 is the position of the first PropertyName field.
    Property = P1;

    current_titlelabel = get(monaxe,titlelabel)
    arginPos = arginPos + 1;  // position of the propertyName in the caller argin list
    while ((Property <> 0) & (Property <= nv-1))
        setTitleLabelProperty(ListArg(Property),ListArg(Property+1),current_titlelabel,current_figure,cur_draw_mode, fname, arginPos)
        Property = Property+2;
        arginPos = arginPos+2;
    end

    //postponed drawings are done now !
    // smart drawnow
    ResetFigureDDM(current_figure, cur_draw_mode);
endfunction

// ============================================================================

function [PName] = getTitleLabelPropertyNam(PropertyName,current_figure,cur_draw_mode)
    // Copyright INRIA
    // GETTITLELABELPROPERTYNAME function
    // is used by the functions title, xlabel, ylabel, zlabel
    // this function gets the matlab properties names and repalce them by the
    // equivalent scilab properties names, using Table and TableEQ

    //conversion to lower format
    str = convstr(PropertyName);


    Table    = [ "tag" "visible" "box" "fontname" "fontangle" "fontsize" ..
    "rotation" "position" "color" "edgecolor" "backgroundcolor" ];

    TableEQ =  [ "tag" "visible" "fill_mode" "font_style" "font_style" "font_size" ..
    "font_angle" "position" "font_foreground" "foreground" "background"];

    opt1=[];
    k=find(part(Table,1:length(str))==str);

    if (isempty(k)) then
        // Scilab names are allowed too...
        k=find(part(TableEQ,1:length(str))==str);
        if ~isempty(k) then
            PName = TableEQ(k(1))
            return;
        end
    end

    if (k == [])
        warning("Error in Property specification : bad argument specified");
        PName=[];
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    elseif (size(k,"*") > 1)
        warning("Ambiguous text property");
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
        warning("Error in Property specification : too much properties specified");
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end

    PName = TableEQ(opt1);
endfunction

// ============================================================================

function [fail] = setTitleLabelProperty(PropertyName,PropertyValue,titlelabel,current_figure,cur_draw_mode, fname, arginPos)
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

    /////////////////////////
    if  PName == "font_size"       // <=> fontsize
        /////////////////////////
        ok =  size(PropertyValue,"*")==1 & or(type(PropertyValue)==[1 10]);
        if ok & type(PropertyValue)==10
            if isnum(PropertyValue)
                PropertyValue =evstr(PropertyValue)   // literal fontsize, like "3"
            else
                ok = %f
            end
        end
        ok = ok & PropertyValue >= 0;
        if ~ok
            warning(msprintf(gettext("%s: Argument #%d: Positive scalar number expected.\n"), fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end
        titlelabel.font_size = PropertyValue
        if PropertyValue~=int(PropertyValue)
            titlelabel.fractional_font = "on"
        else
            titlelabel.fractional_font = "off"
        end
        return

        /////////////////////////
    elseif  PName == "font_angle"  // <=> rotation
        /////////////////////////
        ok =  size(PropertyValue,"*")==1 & or(type(PropertyValue)==[1 10]);
        if ok & type(PropertyValue)==10
            if isnum(PropertyValue)
                PropertyValue =evstr(PropertyValue)   // literal angle, like "30"
            else
                ok = %f;
            end
        end
        if ~ok
            warning(msprintf(gettext("%s: Argument #%d: Scalar number expected.\n"), fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end
        titlelabel.auto_rotation = "off"
        newPropertyValue = modulo(PropertyValue,360)
        titlelabel.font_angle = 360 - newPropertyValue
        return

        /////////////////////////
    elseif or(PName==["font_foreground" "foreground" "background"])
        /////////////////////////
        //  "font_foreground"  <=>  color
        //  "foreground"       <=>  edgecolor, foregroundcolor
        //  "background"       <=>  backgroundcolor
        index = []
        ok = %f
        if size(PropertyValue,"*")==1
            if type(PropertyValue)==10 & isnum(PropertyValue) // literal index like "14"
                PropertyValue = evstr(PropertyValue)
            end
        end
        colors = iscolor(PropertyValue);
        ok = size(colors,1)==1 & ~isnan(colors(1))
        if ok
            if size(colors,2)==3    // RGB
                index = addcolor(colors)
            else
                index = colors
            end
        else
            if type(PropertyValue)==10 & size(PropertyValue,"*")==1 & ..
                grep("none", "/^"+PropertyValue+"/", "r") <> []
                // 'none' selected
                titlelabel.color_mode = 0; // <=> - colormap(1) and not black at all!!
                return
            end
        end
        if ~ok
            msg = gettext("%s: Argument #%d: Wrong color specification.\n");
            warning(msprintf(msg, fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return
        end
        if or(PName==["foreground" "background"])
            titlelabel.fill_mode = "on"
        end
        titlelabel(PName) = index;

        /////////////////////////
    elseif  PName == "text"        // <=> string
        /////////////////////////
        if (type(PropertyValue) == 10)
            titlelabel.text = PropertyValue;
            return
        else
            warning(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    elseif  PName == "position"        // <=> position, à completer: la position par rapport à z n'est pas pris en compte, de plus le vecteur position doit contenir des coordonnées utilisateur (coordonnées selon l'axe)
        /////////////////////////

        if type(PropertyValue) == 1 & size(PropertyValue,"*")<=3
            titlelabel.auto_position = "off";
            titlelabel.position = PropertyValue ;
            return
        else
            warning(msprintf(gettext("%s: Wrong size or type for input argument #%d: A real matrix expected.\n"), fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        /////////////////////////
    elseif  or(PName == ["visible" "fill_mode"])    // <=> visible | box
        /////////////////////////

        if type(PropertyValue) == 10
            if or(PropertyValue == ["off" "of"])
                titlelabel(PName) = "off";
                return
            elseif PropertyValue == "on"
                titlelabel(PName) = "on";
                return
            end
        end
        msg = gettext("%s: Argument #%d: ''on'' or ''off'' expected.\n")
        warning(msprintf(msg, fname, arginPos+1));
        ResetFigureDDM(current_figure, cur_draw_mode);


        /////////////////////////
    elseif  PName == "tag"     //
        /////////////////////////
        if type(PropertyValue) == 10
            titlelabel.tag = PropertyValue;
        else
            msg = gettext("%s: Argument #%d: Text expected.\n")
            warning(msprintf(msg, fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
        end

        /////////////////////////
    elseif PName == "font_style"     // <=> fontangle
        /////////////////////////
        fonts = ["courrier" "symbol" "times" "timesitalic" "timesbold" ..
        "timesbolditalic" "helvetica" "helveticaitalic" "helveticabold"..
        "helveticabolditalic" ]
        if typeof(PropertyValue) == "string" & isnum(PropertyValue)
            PropertyValue = evstr(PropertyValue)    // Literal id, like "3"
        end
        if typeof(PropertyValue) == "string"
            PropertyValue = strsubst(convstr(PropertyValue,"l"), " ", "");
            if or(PropertyValue == fonts)
                titlelabel.font_style = find(fonts==PropertyValue)-1;
            else
                warning(msprintf(_("%s: Wrong value for input argument #%d: At least one ""%s"" expected.\n"), fname, arginPos+1, "font"));
                ResetFigureDDM(current_figure, cur_draw_mode);
                return;
            end
        elseif typeof(PropertyValue) == "constant" & or(PropertyValue==(0:9))
            titlelabel.font_style = PropertyValue;
        else
            warning(msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string matrix expected.\n"), fname, arginPos+1));
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end
    end

endfunction

