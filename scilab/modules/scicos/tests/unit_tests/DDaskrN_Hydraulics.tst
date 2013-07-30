// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DDaskr_Hydraulics_test.zcos"));

// Redefining messagebox() to avoid popup
prot = funcprot();
funcprot(0);
function messagebox(msg, msg_title)
 disp(msg);
endfunction
funcprot(prot);
Info = scicos_simulate(scs_m, list(), 'nw');

// looking for the CLOCK_f/EVTDLY_f to update period
for path_1=1:length(scs_m.objs)
    if typeof(scs_m.objs(path_1))=="Block" & scs_m.objs(path_1).gui=="CLOCK_f" then
        CLOCK_f_scs_m = scs_m.objs(path_1).model.rpar;
        break;
    end
end
for path_2=1:length(CLOCK_f_scs_m)
    if typeof(CLOCK_f_scs_m.objs(path_2))=="Block" & CLOCK_f_scs_m.objs(path_2).gui=="EVTDLY_f" then
        EVTDLY_f_blk = CLOCK_f_scs_m.objs(path_2);
        break;
    end
end

// Start by updating the clock block period (sampling)
scs_m.objs(path_1).model.rpar.objs(path_2).graphics.exprs = [string(5*(10^-3));"0"];

// Modify solver + run DDaskr + save results
scs_m.props.tol(6) = 101;       // Solver
//scs_m.props.tol(1) = 1.0e-10; // abstol
//scs_m.props.tol(2) = 1.0e-10; // reltol
scicos_simulate(scs_m, Info, 'nw');   // DDaskr
ddaskrval = res.values;         // Results
time = res.time;                // Time

// Modify solver + run IDA + save results
scs_m.props.tol(6) = 100;       // Solver
scicos_simulate(scs_m, Info, 'nw');   // IDA
idaval = res.values;            // Results

// Compare results
compa = abs(ddaskrval-idaval);

// Extract mean, standard deviation, maximum
mea = mean(compa);
[maxi, indexMaxi] = max(compa);
stdeviation = st_deviation(compa);

// Verifying closeness of the results
assert_checktrue(maxi <= 10^-(7));
assert_checktrue(mea <= 10^-(7));
assert_checktrue(stdeviation <= 10^-(7));

