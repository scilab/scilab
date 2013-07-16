// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


subdemolist = [_("LQV"),                               "lqv.sce"; ..
_("Spaceship landing trajectory"),      "navet.sce"; ..
_("Computation of optimal parameters"), "seros.sce";];

subdemolist(:,2) = SCI + "/modules/optimization/demos/icse/" + subdemolist(:,2);

