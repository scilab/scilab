// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("datafit.dem.gateway.sce");

subdemolist = ["Non linear data fitting", "datafit.dem.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2)
clear demopath;

