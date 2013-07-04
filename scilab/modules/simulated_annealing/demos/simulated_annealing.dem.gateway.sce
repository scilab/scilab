// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Simulated Annealing"),                   "SAdemo.sce"; ..
_("Simulated Annealing and Ising problem"), "SAIsing2ddemo.sce"];

subdemolist(:,2) = SCI + "/modules/simulated_annealing/demos/" + subdemolist(:,2)
