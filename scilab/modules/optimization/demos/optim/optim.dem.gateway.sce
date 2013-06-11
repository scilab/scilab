// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Rosenbrock"), "optim_rosenbrock.sce"; ...
_("Output"),     "optim_output.sce"; ...
_("Plot"),       "optim_plot.sce"; ...
_("Derivative"), "optim_withderivative.sce"];

subdemolist(:,2) = SCI + "/modules/optimization/demos/optim/"+ subdemolist(:,2);


