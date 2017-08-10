// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function TitleLabel(varargin)
    // TITLELABEL function
    // is used by the functions title, xlabel, ylabel, zlabel

    [lhs,rhs]=argn(0);

    //CurColor = 0; // current color used if no color specified via LineSpec
    // nor PropertyName

    ListArg = varargin;
    titlelabel = ListArg(1);
    ListArg(1) = null(); // remove this parameter from the list

    //detect and set the current axes now:
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
        else
            warning(msprintf(gettext("%s: Wrong type for input argument #%d: Axes handle expected.\n"),"TitleLabel",1));
            return;
        end
    end

    monaxe = gca();

    nv = size(ListArg);
    // detect and set the string now:
    if  type(ListArg(1))== 10 & modulo(nv,2) == 1 then
        st = ListArg(1);
        execstr("monaxe."+ titlelabel +".text"+"=st")
        ListArg(1) = null();
        nv=nv-1
    else
        error(msprintf(gettext("%s: Wrong size or type for input argument #%d: string expected.\n"),"TitleLabel",1));
    end

    T=[];

    //given_data = 2;

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
    current_figure=gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    ///////////////////////////////////
    //Global Property treatment      //
    //PropertyName and PropertyValue //
    ///////////////////////////////////

    // P1 is the position of the first PropertyName field.
    Property = P1;

    current_titlelabel=get(monaxe,titlelabel)

    while ((Property <> 0) & (Property <= nv-1))
        setTitleLabelProperty(ListArg(Property),ListArg(Property+1),current_titlelabel,current_figure,cur_draw_mode)
        //setLabelProperty(ListArg(Property),ListArg(Property+1),monaxe.title,current_figure,cur_draw_mode)
        Property = Property+2;
    end

    //postponed drawings are done now !
    // smart drawnow
    ResetFigureDDM(current_figure, cur_draw_mode);


endfunction

