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
// Check MVC object leaks on Scilab sciblk4 on a simulation.

scicos_log("INFO");
diary("TMPDIR/leaks.log");

exec("SCI/modules/scicos/tests/unit_tests/sciblk4_nwni.tst");
clear

diary("TMPDIR/leaks.log", "close");

log = mgetl("TMPDIR/leaks.log");
log = log(grep(log, ["objectCreated", "objectDeleted"]));
objects = strtod(csvTextScan(log, " ", ".", "string")(:, 7));
objects = gsort(objects);

leaks = objects(2*find((objects(1:2:$-1) - objects(2:2:$))));
if leaks <> [] then
    error("found leaks at " + string(leaks));
end


