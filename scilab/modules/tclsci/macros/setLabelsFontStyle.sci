// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

