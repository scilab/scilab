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

function [Color,Line,LineStyle,Marker,MarkerStyle,MarkerSize,fail]=getLineSpec(str,current_figure,cur_draw_mode)

    LineStyle=1;
    Color=[];
    MarkerStyle=[];
    MarkerSize=1;
    LineStyle=1;
    fail=0;

    //conversion to lower format
    str = convstr(str);

    // 1) LINE STYLE
    // Line type modifiers

    if strindex(str,"--")
        str=strsubst(str,"--","");
        LineStyle=2;
        Line = %T;
    elseif strindex(str,"-.")
        str=strsubst(str,"-.","");
        LineStyle=4;
        Line = %T;
    elseif strindex(str,":")
        str=strsubst(str,":","");
        LineStyle=5;
        Line = %T;
    elseif strindex(str,"-")
        str=strsubst(str,"-","");
        LineStyle=1;
        Line = %T;
    end

    //

    //disp("str vaut:::::::: 1/2")
    //disp(str)

    // 2) COLORS + MARKS STYLE

    opt1=[];

    //Marks
    Table = [ "+" "o" "*" "." "x" "square" "diamond" "^" "v" ">" "<" "pentagram"];
    MarksStyleVal=[1 9 10 0 2 11 5 6 7 12 13 14];
    //MarksSizeVal =[4 3 7  1 3  3 4 3 3  3  3  3];
    //MarksSizeVal =[6 6 6  6 6  6 6 6 6  6  6  6]; // size is given in points now (25.02.05)

    //Colors
    Table= [Table "red" "green" "blue" "cyan" "magenta" "yellow" "black" "k" "white"]
    ColorVal   = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "black" "white"]

    //color management
    //difficulty here since we have to allow every kind of writings:
    //i.e:
    //magenta can be set by: 'm' 'ma' 'mag' 'mage' 'magen' 'magent' or at least 'magenta'
    //


    str = str+"@";

    while length(str) > 1
        c1 = part(str,1); // We get the first character
        k=find(part(Table,1)==c1);

        if (k == [] | (size(k,"*") > 1 & c1 <> "b"))
            ResetFigureDDM(current_figure, cur_draw_mode);
            error(msprintf(gettext("%s: Wrong type for input argument.\n"), "getLineSpec"));
        end

        if c1=="b" // special case here : we have to distinguish between black and blue colors
            c2 = part(str,2);
            if (c2 == "l")
                c3 = part(str,3);
                if (c3 == "a")
                    k=19; // k is set to black color
                else
                    k=15; // k is set to blue color
                end
            else
                k=15; // k is set to blue color
            end
        end

        opt = Table(k);
        for i=1:length(str)
            if part(opt,i) <> part(str,i)
                break;
            end
        end

        opt1 = [opt1 k];

        str = part(str,i:length(str));

    end


    // LineSpec is parsed now
    //Marker = %F;
    //Line   = %T;

    for i=1:size(opt1,"*")

        if (opt1(i) <= 12)
            Marker = %T;
            MarkerStyle = MarksStyleVal(opt1(i));
            MarkerSize  = 6;
            //MarkerSize  = MarksSizeVal (opt1(i));
            //    disp("MarkerSize =");
            //    disp(MarkerSize);
        else
            Color = color(ColorVal(opt1(i)-12));
        end

    end


endfunction
// end of getLineSpec
