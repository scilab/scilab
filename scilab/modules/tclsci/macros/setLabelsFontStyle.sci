// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function setLabelsFontStyle(label,ftn)
    global ged_handle; h=ged_handle
    select label
    case "t"
        TCL_EvalStr("Notebook:raise $uf.n Z");
        h.title.font_style=find(ftn==["Monospaced" "Symbol" "Serif",..
        "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
        "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
        "SansSerif Bold Italic"])-1;
    case "x"
        h.x_label.font_style=find(ftn==["Monospaced" "Symbol" "Serif",..
        "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
        "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
        "SansSerif Bold Italic"])-1;
    case "y"
        h.y_label.font_style=find(ftn==["Monospaced" "Symbol" "Serif",..
        "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
        "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
        "SansSerif Bold Italic"])-1;
    case "z"
        h.z_label.font_style=find(ftn==["Monospaced" "Symbol" "Serif",..
        "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
        "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
        "SansSerif Bold Italic"])-1;
    end;
endfunction

