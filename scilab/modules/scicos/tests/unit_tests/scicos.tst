//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

if exists("scicos_scicoslib") == 0 then
    load("SCI/modules/scicos/macros/scicos_scicos/lib");
end
if exists("scicos_utilslib") == 0 then
    load("SCI/modules/scicos/macros/scicos_utils/lib");
end
if or(~isdef(["modelica_libs" "scicos_pal_libs"])) then
    [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables()
end

exec(loadpallibs,-1)

exec("SCI/modules/scicos/tests/unit_tests/scicos_tests.sci");

options = default_options();
//build the block set
blockslib = "scs" + ["Branching" "Events" "Misc" "Sinks" "Threshold" "Linear" ...
"NonLinear" "Sources" "Electrical" "Hydraulics"] + "lib";
Blocs = [];
for blocklib=blockslib
    B = string(blocklib);
    Blocs = [Blocs; B(2:$)];
end
Blocs(Blocs=="m_sin") = [];

nb = size(Blocs, 1);
assert_checkfalse(checkdefine());
assert_checkfalse(checkinputs());
assert_checkfalse(checkoutputs());
assert_checkfalse(checkorigin());
assert_checkfalse(checkdeput());
assert_checkfalse(checkfiring());
assert_checkfalse(check_define_ports());
assert_checkfalse(check_set_ports());
