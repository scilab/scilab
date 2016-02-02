//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

//
// This is the demonstration script of xstringb
// used when calling xstringb without any parameter.
//

function [] = %_xstringb()
    scf();
    axes = gca();
    axes.axes_visible = "on";
    str = ["Scilab","is";"not","Esilab"];
    xstringb(0.1,0.1,str,0.5,0.5,"fill");
    txt = gce();
    txt.box = "on";
endfunction
