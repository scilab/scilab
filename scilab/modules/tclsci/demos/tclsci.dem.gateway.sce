// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("tclsci.dem.gateway.sce");
    add_demo(gettext("Tcl/Tk"), demopath + "tclsci.dem.gateway.sce");

    subdemolist = [_("Color")  ,"color.dem.sce"      ; ..
    _("Puzzle") ,"puzzle.dem.sce"     ; ..
    _("Scroll") ,"scroll.dem.sce"     ; ..
    _("Scale")  ,"scale.dem.sce"      ]

    subdemolist(:,2) = demopath + "/tk/" + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
