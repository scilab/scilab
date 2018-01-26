// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

demopath = get_absolute_file_path("histBarStem.dem.gateway.sce");

subdemolist=[
    _("Stems (plot2d3)")     , "plot2d3.dem.sce"
    _("Bars (custom)")       , "bars_custom.dem.sce"
    _("Bars (grouped)")      , "bar.dem.sce"
    _("histplot")            , "histplot.dem.sce"
    _("bar3d")               , "bar3d.dem.sce"
    ];


subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
