//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

//
// Sliding pendulum, curve #1
//

getf("SCI/demos/simulation/dae/dae2/pendg.sci")
getf("SCI/demos/simulation/dae/dae2/pendc1.sci")

getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');

demo_message('SCI/demos/simulation/dae/dae2/pendg1_1.sce');

demo_run('SCI/demos/simulation/dae/dae2/pendg1_1.sce');
