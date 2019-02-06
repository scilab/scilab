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
// Check MVC object leaks on a Scilab simulation.

loadXcosLibs();
diary("TMPDIR/leaks.log");
scicos_log("INFO");

scs_m = scicos_diagram();
scs_m.props.tf = 0.5;

scs_m.objs(1) = GENSIN_f("define");
scs_m.objs(1).graphics.pout = 5;
scs_m.objs(2) = GAINBLK("define");
scs_m.objs(2).graphics.pin = 5;
scs_m.objs(2).graphics.pout = 6;
scs_m.objs(3) = TRASH_f("define");
scs_m.objs(3).graphics.pin = 6;
scs_m.objs(3).graphics.pein = 7;
scs_m.objs(4) = CLOCK_c("define");
scs_m.objs(4).graphics.peout = 7;

scs_m.objs(5) = scicos_link(from=[1 1 0], to=[2 1 1]);
scs_m.objs(6) = scicos_link(from=[2 1 0], to=[3 1 1]);
scs_m.objs(7) = scicos_link(from=[4 1 0], to=[3 1 1], ct=[1 -1]);

Info = scicos_simulate(scs_m, list());
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
