//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("optimization.dem.gateway.sce");
subdemolist = ["Non linear data fitting"       , "datafit/datafit.dem.sce"       ];
subdemolist(:,2) = demopath + subdemolist(:,2)
