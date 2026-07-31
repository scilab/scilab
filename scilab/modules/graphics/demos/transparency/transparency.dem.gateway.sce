// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("transparency.dem.gateway.sce");

subdemolist = [                                   ..
_("Fec")               , "fec.dem.sce"        ; ..
_("Fonts")             , "fonts.dem.sce"      ; ..
_("Grayplot")          , "grayplot.dem.sce"   ; ..
_("Markers")           , "markers.dem.sce"    ; ..
_("Matplot")           , "matplot.dem.sce"    ; ..
_("Plot3d")            , "plot3d.dem.sce"     ; ..
_("Polyline")          , "polyline.dem.sce"   ; ..
_("Scatter")           , "scatter.dem.sce"    ; ..
_("Sphere & Cylinder") , "sphere.dem.sce"     ; ..
_("Surface")           , "surface.dem.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
