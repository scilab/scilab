// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- XCOS TEST -->
//

ilib_verbose(0); //to remove ilib_* traces

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/DAE/Hydraulics.zcos"));

IDA_val_ref = [ 0.2014730
0.2014678
0.2014630
0.2014586
0.2014545
0.2014507
0.2014472
0.2014440
0.201441
0.2014383
0.2014358
0.2014336
0.2014315
0.2014297
0.201428
0.2014265
0.2014252
0.2014240
0.2014229
0.2014220
0.2014211
0.2014204
0.2014198
0.2014192
0.2014188
0.2014184
0.2014181
0.2014178
0.2014176
0.2014174
0.2014173
0.2014172
0.2014172
0.2014172
0.2014172
0.2014172
0.2014173
0.2014174
0.2014174
0.2014175
0.2014176
0.2014178
0.2014179
0.201418
0.2014181
0.2014183
0.2014184
0.2014185
0.2014186
0.2014188
0.2014189
0.2014190
0.2014191
0.2014193
0.2014194
0.2014195
0.2014196
0.2014197
0.2014198
0.2014199
0.2014200
0.2014200
0.2014201
0.2014202
0.2014203
0.2014203
0.2014204
0.2014204 ];
IDA_time_ref = (25.05:.05:50)';

scs_m.props.tol(6) = 100; // Solver
scs_m.props.context = "per = 0.05"; // Period, to get more varied results
scicos_simulate(scs_m);   // IDA

assert_checkalmostequal(res.values($-67:$, :), IDA_val_ref, [], 1d-7);
assert_checkalmostequal(res.time, IDA_time_ref);
