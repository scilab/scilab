//
// Scilab ( http://www.scilab.org/ ) 
// This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("old_demos.dem.gateway.sce");

subdemolist = ["Old Gain Block"                              , "old_gain_block.dem.sce"  ];

subdemolist(:,2) = demopath + subdemolist(:,2);
