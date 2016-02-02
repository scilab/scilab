// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function plot(varargin)
    // Try to build a new better parser that could manage things like:
    // plot(x,y,'X',1:10); // where X stands for Xdata (Matlab recognizes
    //it and treats it well...)

    [lhs,rhs]=argn(0);

    if ~rhs
        //LineSpec and PropertySpec examples:
        t = 0:%pi/20:2*%pi;
        tt = t';
        drawlater();
        subplot(211);
        plot(tt, sin(tt), "ro-.", tt, cos(tt), "cya+", tt, abs(sin(tt)), "--mo");
        subplot(212);
        plot([t ;t],[sin(t); cos(t)],"xdat",[1:2]);
        drawnow();
        return;
    end



    CurColor = 0; // current color used if no color specified via LineSpec
    // nor PropertyName



    ListArg = varargin;

    //detect and set the current axes now:
    if type(ListArg(1)) == 9
        hdle = ListArg(1);
        if (hdle.type == "Axes")
            sca(ListArg(1));
            ListArg(1) = null(); // remove this parameter from the list
        else
            warning("Handle should be an Axes handle")
            return;
        end
    end


    nv = size(ListArg)


    argTypes=[];
    couple=[];

    typeOfPlot = "plot";
    provided_data = 2;

    for curArgIndex=1:nv
        argTypes(curArgIndex,1) = type(ListArg(curArgIndex))
    end

    Ttmp=argTypes;

    for i=1:nv-1
        acceptedTypes=[];
        acceptedTypes=find(Ttmp(i,1)==1 & or(Ttmp(i+1,1)==[1,13,130])) // to accept double, macro function or primitive as second argument

        if (acceptedTypes<>[]) then
            couple=[couple i];
            Ttmp(i,1)  = 99; // Replace a known type by 99 (no meaning) to count it once only!
            Ttmp(i+1,1)= 99; // to avoid having (x1,y1,x2,y2) ->couple=[1,2,3]
            // With this trick, couple=[1,3];
        end

    end


    if (couple==[]) // No data couple found
        // Search for at least a single data , i.e.: plot(y)

        if ((argTypes(1,1)==1 | argTypes(1,1)==8) & ListArg(1)<>[]) then // case plot(SINGLE y,...)
            couple = 1;
            provided_data = 1;

            if (modulo(nv-couple,2)<>0) then
                P1 = couple+2 // Position of the first PropertyName field
            else
                P1 = couple+1
            end

        else
            warning("Error inside input argument : no data");
            return;
        end

    else

        // Some test to check wrong inputs
        //
        // 1. Test if 2 data couples (first : type==1, second : type=[1,13,130])
        // are at least separated by 2 indices
        if (size(couple, "*") > 1 && couple(2:$)-couple(1:$-1)<2)
            warning("Error inside input argument !");
            return;
        end

        // 2. Test if no string couples happen before P1 (see below for P1 definition)
        for index=1:couple($)
            acceptedTypes=[];
            acceptedTypes=find(Ttmp(index,1)==10 & Ttmp(index+1,1)==10)

            if (acceptedTypes<>[]) then
                warning("Error inside input argument : String argument is an unknown option.");
                return;
            end
        end



        if (modulo(nv-(couple($)+1),2)<>0) then
            P1 = couple($)+3 // Position of the first PropertyName field
        else
            P1 = couple($)+2
        end

    end

    numplot = size(couple,"*");

    xyIndexLineSpec = zeros(numplot,3);
    // xyIndexLineSpec is a matrix storing the index of x, y and linespec
    // if one of these indices is 0 => it does not exist
    // (which is possible for x and linepsec, not for y)

    if (provided_data == 2) then

        for curCouple=1:size(couple,"*")
            xyIndexLineSpec(curCouple,1:2) = couple(curCouple) +[0,1] // x,y index storage

            if (couple(curCouple)+2 < P1)
                if (argTypes(couple(curCouple)+2,1)==10) then // LineSpec treatment
                    xyIndexLineSpec(curCouple,3) = couple(curCouple)+2;
                end
            end
        end
    else
        // we are in the case where: plot(SINGLE y,... x not specified
        // or plot(handle,SINGLE y,...
        xyIndexLineSpec(1,1) = 0; // no x specified
        xyIndexLineSpec(1,2) = couple;

        //pause;

        if (couple+1 < P1)
            if (argTypes(couple+1,1)==10) then // LineSpec treatment
                xyIndexLineSpec(1,3) = couple+1;
            end
        end
    end



    // delay the drawing commands
    // smart drawlater
    current_figure=gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    // check whether this is the first plot for the axes in which we will draw
    curAxes = gca();
    // save auto_clear state.
    OldAutoClear = curAxes.auto_clear;

    isFirstPlot = (curAxes.children == [])

    //Now, we plot the decomposed plots one by one with their own linespec
    // provided_data = 2 : x and y are provided

    FinalAgreg=[]; // Final Compound containing all the new created plots.

    //for i=numplot:-1:1
    for i=1:numplot
        // Set off auto_clear for allowing multiple graphics entity
        // will be restored behond
        if i>1 then
            curAxes.auto_clear="off";
        end

        //default values
        Marker=[];
        MarkerSize=1;
        Color=[];
        LineStyle=1;
        Line = %F;
        Marker = %F;

        if (provided_data == 2) then

            if (type(ListArg(xyIndexLineSpec(i,2))) == 13 | type(ListArg(xyIndexLineSpec(i,2))) == 130)
                // A function (macro or primitive) is given. We need to build the vector or matrix.
                sizefirstarg = size(ListArg(xyIndexLineSpec(i,1)));
                buildFunc = ListArg(xyIndexLineSpec(i,2));
                firstarg = ListArg(xyIndexLineSpec(i,1));
                tmp = [];

                for ii=1:sizefirstarg(1,2)
                    for jj=1:sizefirstarg(1,1)

                        // function evaluation may fail
                        // try/cacth is buggy for now
                        // so use execstr until the bug is fixed
                        err = execstr("tmp(jj,ii) = buildFunc(firstarg(jj,ii))","errcatch","n");

                        if (err <> 0) then
                            // reset data
                            ResetFigureDDM(current_figure, cur_draw_mode);

                            // get error
                            [err_message, err_number, err_line, err_func] = lasterror(%t);

                            clear buildFunc;
                            // print it
                            if (err_func <> "") then
                                // ascii(10) = \n
                                error(msprintf(gettext("%s: Error : unable to evaluate input function ''%s''.") + ascii(10) + gettext("Error %d at line %d of the function: ''%s''"), "plot", err_func,err_number, err_line, err_message));
                            else
                                error(msprintf(gettext("%s: Error : unable to evaluate input function.") + ascii(10) + gettext("Error %d at line %d of the function: ''%s''"), "plot", err_number, err_line, err_message));
                            end
                            // exit function
                            return;
                        end

                    end
                end


                ListArg(xyIndexLineSpec(i,2)) = tmp;
                // if there is an other iteration, we will have error message redefining function.
                // we need to clear here and not before, because user must see the warning if needed.
                clear buildFunc;
            end
            [X,Y] = checkXYPair(typeOfPlot,ListArg(xyIndexLineSpec(i,1)),ListArg(xyIndexLineSpec(i,2)),current_figure,cur_draw_mode)
        else
            if or(size(ListArg(xyIndexLineSpec(1,2)))==1)  // If this is a vector
                if size(ListArg(xyIndexLineSpec(1,2)), "r") == 1 then
                    X=1:length(ListArg(xyIndexLineSpec(1,2))); // insert a column abcsissa vector of same length,
                else
                    X=(1:length(ListArg(xyIndexLineSpec(1,2))))'; // insert a row abcsissa vector of same length,
                end
            else                                  // if this is a matrix,
                X=(1:size(ListArg(xyIndexLineSpec(1,2)),1))'; // insert a row abcsissa vector with same size
            end
            // In both cases (matrix/vector), transpose it now so no warning is issued in checkXYPair().
            [X,Y] = checkXYPair(typeOfPlot,X,ListArg(xyIndexLineSpec(1,2)),current_figure,cur_draw_mode)
        end

        // Case if 'Xdata', 'Ydata' or 'Zdata' have been set in (PropertyName,Propertyvalue) couples
        // must be taken into account now

        // P1 is the position of the first PropertyName field.
        Property = P1;

        while (Property <= nv-1)
            PropertyName  = ListArg(Property);
            PropertyValue = ListArg(Property+1);

            // Xdata can ONLY be a vector (cf. Matlab help)
            PName = getPlotPropertyName(PropertyName,current_figure,cur_draw_mode);
            if (PName == "xdata")

                if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
                    warning("Xdata value must be a column or row vector.");
                    ResetFigureDDM(current_figure, cur_draw_mode);
                    return;
                else
                    PropertyValue = PropertyValue(:); // force
                    if or(size(X))==1  // If X is a vector (inevitably a column vector because checkXYPair always returns a column vector)
                        X = PropertyValue; // X is replaced by PropertyValue
                        [X,Y] = checkXYPair(typeOfPlot,X,Y,current_figure,cur_draw_mode)
                    else // X is a matrix
                        if size(PropertyValue,"*") == size(X,1)
                            for j=1:size(PropertyValue,"*")
                                X(j,:) = PropertyValue(j,1);
                            end
                        else
                            str="plot : incompatible dimensions in input arguments";
                            warning(str);
                            ResetFigureDDM(current_figure, cur_draw_mode);
                        end
                    end
                end

                // Ydata ONLY be a vector (contrary to what is said by the Matlab help)
            elseif (PName == "ydata")

                if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
                    warning("Ydata value must be a column or row vector.");
                    ResetFigureDDM(current_figure, cur_draw_mode);
                    return;
                else
                    PropertyValue = PropertyValue(:); // force
                    if or(size(Y))==1  // If Y is a vector (inevitably a column vector because checkXYPair always returns a column vector)
                        Y = PropertyValue; // Y is replaced by PropertyValue
                        [X,Y] = checkXYPair(typeOfPlot,X,Y,current_figure,cur_draw_mode)
                    else // Y is a matrix
                        if size(PropertyValue,"*") == size(Y,1)
                            for j=1:size(PropertyValue,"*")
                                Y(j,:) = PropertyValue(j);
                            end
                        else
                            str="plot : incompatible dimensions in input arguments";
                            warning(str);
                            ResetFigureDDM(current_figure, cur_draw_mode);
                        end
                    end

                end

                // Zdata will be treated after plot building
            end

            Property = Property+2;
        end



        //Now we have an array xyIndexLineSpec [numplot x 3] containing indices pointing on T for :
        // - x (<>0 if existing)
        // - y
        // - linespec (<>0 if existing)
        // for each plot passed in argument
        //       x | y | linespec
        //       ----------------
        //plot1   0|i1 |0    <=> plot(y)
        //plot2  i2|i3 |0    <=> plot(x,y)
        //plot3  i4|i5 |i6   <=> plot(x,y,LINESPEC)
        //...



        if (xyIndexLineSpec(i,3)<>0) then // if we have a line spec <=> index <> 0
            [Color,Line,LineStyle,Marker,MarkerStyle,MarkerSize,fail] = getLineSpec(ListArg(xyIndexLineSpec(i,3)),current_figure,cur_draw_mode);
        end

        // The plot is made now :
        err = execstr("plot2d(X,Y)","errcatch","m");

        if err <> 0
            mprintf("Error %d : in plot2d called by plot",err);
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
        end

        agreg=gce();  // when using plot2d, we always have an Compound as the current entity

        FinalAgreg = [agreg FinalAgreg];

        if Color==[]
            DefaultColor = %T;
        else
            DefaultColor = %F;
        end

        for ii=size(agreg.children,"*"):-1:1
            curPolyline=agreg.children(ii); // we apply linespec to the lines

            // Color treatment : if no color specified by LineSpec nor PropertyName
            // Set the default color to the curve
            if DefaultColor == %T
                [Color,CurColor] = setDefaultColor(CurColor);
            end

            if (Marker == %T)
                curPolyline.mark_style=MarkerStyle;
                curPolyline.mark_mode ="on";
                curPolyline.mark_foreground = Color;
                curPolyline.mark_style=MarkerStyle;
                curPolyline.mark_size=MarkerSize;
            else
                curPolyline.mark_mode ="off"
            end

            if (Line == %T)
                curPolyline.line_mode="on";
                curPolyline.foreground = Color;
                curPolyline.line_style = LineStyle;
            else
                curPolyline.line_mode="off"
            end

            if (Line == %F & Marker ==%F) // no linespec nor PropertyName set
                curPolyline.line_mode="on";
                curPolyline.foreground = Color;
                curPolyline.line_style = LineStyle;
            end

        end
    end

    //Reset auto_clear Property
    curAxes.auto_clear = OldAutoClear;

    ///////////////////////////////////
    //Global Property treatment      //
    //PropertyName and PropertyValue //
    ///////////////////////////////////



    // Those properties will be applied to Agreg children
    Agreg = glue(FinalAgreg(1:$))

    nbCompound = find(Agreg.children.type=="Compound")

    while (nbCompound<>[])
        nbCompound=nbCompound(1);
        unglue(Agreg.children(nbCompound));
        nbCompound=find(Agreg.children.type=="Compound")
    end



    // P1 is the position of the first PropertyName field.
    Property = P1;

    Curves = Agreg.children
    //Curves(:,1) = Curves(:,$:-1:1);

    // set mark_size_unit to 'point' for all the curves
    Curves.mark_size_unit="point";

    while (Property <= nv-1)
        setPlotProperty(ListArg(Property),ListArg(Property+1),Curves,current_figure,cur_draw_mode)

        Property = Property+2;
    end

    // force drawing of box like in matlab
    // for a first plot
    // unless we are using centered axes
    // to keep compatibility with Scilab 4
    if  isFirstPlot & curAxes.x_location <> "origin" & curAxes.y_location <> "origin" then
        curAxes.box = "on";
    end



    //postponed drawings are done now !
    // smart drawnow
    ResetFigureDDM(current_figure, cur_draw_mode)

endfunction
