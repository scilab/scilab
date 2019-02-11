// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// Check MVC object leaks on Scilab adapters API on a simulation sub-pass.

log = mgetl("SCI/modules/scicos/tests/unit_tests/do_eval.dia.ref");

log = log(grep(log, ["objectCreated", "objectDeleted"]));
objects = strtod(csvTextScan(log, " ", ".", "string")(:, 7));
objects = gsort(objects);

leaks = objects(2*find((objects(1:2:$-1) - objects(2:2:$))));
if leaks <> [] then
    error("found leaks at " + string(leaks));
end


