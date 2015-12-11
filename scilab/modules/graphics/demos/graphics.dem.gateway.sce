// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("graphics.dem.gateway.sce");
    add_demo(gettext("Graphics"), demopath+"graphics.dem.gateway.sce");

    subdemolist = [_("2D and 3D plots")             ,"2d_3d_plots/2d_3d_plots.dem.gateway.sce"             ; ..
    _("Basic functions")             ,"basic_functions/basic_functions.dem.gateway.sce"     ; ..
    _("Animation")                   ,"anim/anim.dem.gateway.sce"                           ; ..
    _("Finite Elements")             ,"fec/fec.dem.gateway.sce"                             ; ..
    _("Bezier curves and surfaces")  ,"bezier/bezier.dem.gateway.sce"                       ; ..
    _("More surfaces")               ,"surface/surfaces.dem.gateway.sce"                    ; ..
    _("Complex elementary functions"),"cmplxfunc/cmplxfunc.dem.gateway.sce"                 ; ..
    _("bar histogram")               ,"bar/bar.dem.sce"; ..
    _("Datatips")                    ,"datatips/datatips.dem.gateway.sce"                   ; ..
    _("LaTeX/MathML")                ,"textrendering/textrendering.dem.gateway.sce"         ; ..
    _("Lighting")                    ,"lighting/lighting.dem.gateway.sce"                   ; ..
    _("Misc")                        ,"misc/misc.dem.sce"                                   ; ..
    _("Colormap")                    ,"colormap/colormap.dem.sce"                           ; ..
    _("Matplot")                    ,"matplot/matplot.dem.gateway.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
