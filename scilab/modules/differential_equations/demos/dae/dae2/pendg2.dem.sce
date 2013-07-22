// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

//
// Sliding pendulum, curve #2
//

function demo_pendg2()

    exec(SCI+"/modules/differential_equations/demos/dae/dae2/pendg.sci");
    exec(SCI+"/modules/differential_equations/demos/dae/dae2/pendc1.sci");
    exec(SCI+"/modules/differential_equations/demos/dae/dae2/demo_sliding_pendulum.sci");

    demo_sliding_pendulum()

endfunction

demo_pendg2();
clear demo_pendg2;