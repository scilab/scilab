// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


subdemolist = [
    _("Parabolic model (3 params)"), "datafit.dem.sce"
    _("Sloping gaussian model (5 params)"), "slopingGaussian.dem.sce"
    _("Tilted ellipse (5 params)"), "ellipse.dem.sce"
    ];
subdemolist(:,2) = SCI + "/modules/optimization/demos/datafit/" + subdemolist(:,2);
