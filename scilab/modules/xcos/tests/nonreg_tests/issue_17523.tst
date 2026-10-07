// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17523 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17523
//
// <-- Short Description -->
// xcos(scs_m) opened an empty diagram.

// Checks that xcos(scs_m) displays the blocks and links of an imported
// diagram, and keeps its simulation parameters.
loadXcosLibs(); loadScicos();
scs_m = scicos_diagram();
scs_m.props.tf = 30;
scs_m.objs(1) = CSCOPE("define");
scs_m.objs(2) = CONST("define");
scs_m.objs(3) = CLOCK_c("define");
scs_m.objs(4) = scicos_link(xx=[0;0], yy=[0;0], from=[2 1 0], to=[1 1 1], ct=[1 1]);
scs_m.objs(5) = scicos_link(xx=[0;0], yy=[0;0], from=[3 1 0], to=[1 1 1], ct=[5 -1]);

f = TMPDIR + "/issue_17523.zcos";
xcosDiagramToScilab(f, scs_m);
clear scs_m
importXcosDiagram(f);

xcos(scs_m);

// cells are inserted by a background worker
jimport org.scilab.modules.xcos.Xcos;
instance = Xcos.getInstance();
diags = instance.openedDiagrams();
assert_checkequal(diags.size(), int32(1));
d = diags.get(0);
model = d.getModel();
for i = 1:100
    n = model.getChildCount(d.getDefaultParent());
    if n == 5 then
        break
    end
    sleep(100);
end
assert_checkequal(n, int32(5));
assert_checkequal(scs_m.props.tf, 30);

closeXcos();
