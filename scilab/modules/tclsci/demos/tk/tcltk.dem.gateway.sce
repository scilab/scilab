// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


subdemolist = [_("Color")  ,"color.dem.sce"      ; ..
               _("Puzzle") ,"puzzle.dem.sce"     ; ..
               _("Scroll") ,"scroll.dem.sce"     ; ..
               _("Scale")  ,"scale.dem.sce"      ]

subdemolist(:,2) = SCI + "/modules/tclsci/demos/tk/" + subdemolist(:,2);
