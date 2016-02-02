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


function [PName] = getPlotPropertyName(PropertyName,current_figure,cur_draw_mode)

    //conversion to lower format
    err = execstr("str = convstr(PropertyName);", "errcatch", "m");

    if err <> 0
        warning(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"getPlotPropertyName",1));
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end

    Table    = ["clipping" "color"      "foreground" "linestyle" "linewidth" "thickness" "marker" ..
    "markstyle" "markeredgecolor" "markforeground" "markerfacecolor"  "markbackground" ..
    "markersize" "marksize" "visible" "xdata" "ydata" "zdata"];
    TableEQ =  ["clipping" "foreground" "foreground" "linestyle" "thickness" "thickness" "markstyle"..
    "markstyle"  "markforeground" "markforeground" "markbackground" "markbackground" ..
    "marksize"   "marksize" "visible" "xdata" "ydata" "zdata"]; // equivalent Table

    opt1=[];

    k=find(part(Table,1:length(str))==str);

    if (k == [])
        warning(msprintf(gettext("%s: Wrong value for input argument #%d: A valid property name expected.\n"),"getPlotPropertyName",1));
        PName=[];
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    elseif ( size(k,"*") > 1)
        if or(find(k==7)) // case PropertyName == 'marker'
            k=7; // set to PropertyName == 'marker' only
        else
            warning(msprintf(gettext("%s: Ambiguous line property.\n"),"getPlotPropertyName"));
            PName=[];
            ResetFigureDDM(current_figure, cur_draw_mode);
            return;
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

    if (size(opt1,"*") > 1)
        warning(msprintf(gettext("%s: Wrong size for input argument #%d: Only one property expected.\n"),"getPlotPropertyName",1));
        ResetFigureDDM(current_figure, cur_draw_mode);
        return;
    end

    PName = TableEQ(opt1);

endfunction
