// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17581 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17581
//
// <-- Short Description -->
// The CMSCOPE "Name of Scope" was not propagated to the figure when it was not a C identifier
//

loadXcosLibs();

function check_scope_name(gui, nin, name_idx)
    scs_m = scicos_diagram();
    scs_m.props.tf = 1;
    scs_m.objs(1) = evstr(gui + "(""define"")");
    scs_m.objs(1).graphics.exprs(name_idx) = "My scope é";
    scs_m.objs(2) = CLOCK_c("define");
    scs_m.objs(3) = CLINDUMMY_f("define");
    scs_m.objs($+1) = scicos_link(from=[2 1 0], to=[1 1 1], ct=[5 -1]);
    for i = 1:nin
        scs_m.objs($+1) = RAMP("define");
        scs_m.objs($+1) = scicos_link(from=[size(scs_m.objs), 1, 0], to=[1 i 1]);
    end

    scicos_simulate(scs_m, list());

    f = gcf();
    assert_checkequal(f.figure_name, "My scope é");
    close(f);
endfunction

check_scope_name("CSCOPE", 1, 10);
check_scope_name("CMSCOPE", 2, 11);
check_scope_name("SCALE_CSCOPE", 1, 10);
check_scope_name("SCALE_CMSCOPE", 2, 11);
