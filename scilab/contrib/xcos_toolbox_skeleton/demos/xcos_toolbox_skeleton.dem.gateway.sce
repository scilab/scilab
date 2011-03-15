// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("xcos_toolbox_skeleton.dem.gateway.sce");

subdemolist = [];

subdemolist(:,2) = demopath + subdemolist(:,2);
