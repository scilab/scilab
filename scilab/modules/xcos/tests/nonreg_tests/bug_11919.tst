// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 11919 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11919
//
// <-- Short Description -->
// dsslti4 crash Scilab when used without input or output



function model = create_test_dlss_blk(x0, A, B, C, D, in, out)

    // create model
    model = scicos_model();
    model.sim = list('dsslti4',4);
    model.in = in;
    model.out = out;
    model.evtin = 1;
    model.dstate = x0(:);
    model.rpar = [A(:);B(:);C(:);D(:)];
    model.blocktype = 'd';
    model.dep_ut = [%f %f];

    // test model
    blk = model2blk(model);
    callblk(blk, 4, 0);
endfunction


// test model with no input
x0 = 0;
A = -1;
B = [];
C = 1;
D = 0;
in = [];
out = 1;
create_test_dlss_blk(x0, A, B, C, D, in, out);

// test model with no output
x0 = 0;
A = -1;
B = 1;
C = [];
D = 0;
in = 1;
out = [];
create_test_dlss_blk(x0, A, B, C, D, in, out);

// test model with no input and no output
x0 = 0;
A = -1;
B = [];
C = [];
D = 0;
in = [];
out = [];
create_test_dlss_blk(x0, A, B, C, D, in, out);

// test model with no input, no output and no discrete state
x0 = [];
A = [];
B = [];
C = [];
D = [];
in = [];
out = [];
create_test_dlss_blk(x0, A, B, C, D, in, out);

// test default model
x0 = 0;
A = -1;
B = 1;
C = 1;
D = 0;
in = 1;
out = 1;
create_test_dlss_blk(x0, A, B, C, D, in, out);

