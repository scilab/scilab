//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("graphics.dem.gateway.sce");

subdemolist = ["2D and 3D plots"             ,"2d_3d_plots/2d_3d_plots.dem.gateway.sce"             ; ..
               "Basic functions"             ,"basic_functions/basic_functions.dem.gateway.sce"     ; ..
               "Animation"                   ,"anim/anim.dem.gateway.sce"                           ; ..
               "Finite Elements"             ,"fec/fec.dem.gateway.sce"                             ; ..
               "Bezier curves and surfaces"  ,"bezier/bezier.dem.gateway.sce"                       ; ..
               "More surfaces"               ,"surface/surfaces.dem.gateway.sce"                    ; ..
               "Complex elementary functions","cmplxfunc/cmplxfunc.dem.gateway.sce"                 ; ..
               "bar histogram"               ,"bar/bar.dem.sce"; ..
               "LaTeX/MathML"                ,"textrendering/textrendering.dem.gateway.sce"                                     ];



if ~ usecanvas() then
    subdemolist = [ subdemolist ;
                "Misc"                        ,"misc/misc.dem.sce"                                  ; ..
                "Colormap"                    ,"colormap.dem.sce"                                   ];
end

subdemolist(:,2) = demopath + subdemolist(:,2);
