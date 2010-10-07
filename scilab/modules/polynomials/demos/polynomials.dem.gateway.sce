// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Introduction")             , "intro/poly.intro.dem.sce" ];

subdemolist(:,2) = SCI + "/modules/polynomials/demos/" + subdemolist(:,2);
