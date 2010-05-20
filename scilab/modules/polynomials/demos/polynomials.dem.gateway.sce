// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file is released into the public domain

demopath = get_absolute_file_path("polynomials.dem.gateway.sce");

subdemolist = ["Introduction"             , "intro/poly.intro.dem.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2)
