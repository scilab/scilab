//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("optimization.dem.gateway.sce");
subdemolist = [
"Non linear data fitting"         "datafit/datafit.dem.sce"       
"fminsearch"                      "neldermead/fminsearch.sce"       
"neldermead/Rosenbrock Variable"  "neldermead/neldermead_rosenbrock.sce"       
"neldermead/Output Command"       "neldermead/neldermead_outputcmd.sce"       
"neldermead/Box A"                "neldermead/neldermead_boxproblemA.sce"       
"nmplot/Han #1"             "neldermead/nmplot_han1.sce"       
"nmplot/Han #2"             "neldermead/nmplot_han2.sce"       
"nmplot/McKinnon #1"        "neldermead/nmplot_mckinnon.sce"       
"nmplot/McKinnon #2"        "neldermead/nmplot_mckinnon2.sce"       
"nmplot/Quadratic Fixed #1"   "neldermead/nmplot_quadratic.fixed.sce"       
"nmplot/Quadratic Fixed #2"   "neldermead/nmplot_quadratic.fixed2.sce"       
"nmplot/Rosenbrock Fixed"   "neldermead/nmplot_rosenbrock.fixed.sce"       
"nmplot/Rosenbrock Variable"         "neldermead/nmplot_rosenbrock.sce"       
];
subdemolist(:,2) = demopath + subdemolist(:,2)


