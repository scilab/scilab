// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("bezier.dem.gateway.sce");

exec(demopath+"/Macros.sci");

subdemolist=[
"Gammatest"              , "gammatest.sce";
"2D curve Bezier test"   , "beziertest.sce";
"3D curve Bezier test"   , "bezier3dtest.sce";
"Bezier surface test"    , "beziersurftest.sce"
"Bezier surface test 2"  , "c1test.sce"];


subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
