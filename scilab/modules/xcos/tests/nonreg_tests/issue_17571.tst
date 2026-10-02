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
// <-- Non-regression test for issue 17571 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/work_items/17571
//
// <-- Short Description -->
// The "Unique name" of a SUPER_f block lost its accented characters and a
// name made of digits became empty.

// Any Xcos gateway loads the Xcos jars
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos"));
jimport org.scilab.modules.xcos.JavaController;
jimport org.scilab.modules.xcos.Kind;
jimport org.scilab.modules.xcos.graph.model.XcosCell;
jimport org.scilab.modules.xcos.block.SuperBlock;
jimport com.mxgraph.model.mxGeometry;

cases = ["1234"          "_1234"
         "12ab"          "_12ab"
         "Réseau&Grid"   "Reseau_Grid"
         "my block"      "my_block"
         "  a - b  "     "a_b"
         "a_&b"          "a_b"
         "_x"            "_x"
         "Nom_valide"    "Nom_valide"
         "&&"            ""];
for k = 1:size(cases, 1)
    assert_checkequal(XcosCell.toValidCIdentifier(cases(k, 1)), cases(k, 2));
end

// SUPER_f names go through SuperBlock.setValue
controller = JavaController.new();
uid = controller.createObject(Kind.BLOCK);
block = SuperBlock.new(controller, uid, Kind.BLOCK, "", mxGeometry.new(), "", "issue_17571");
for k = 1:3
    block.setValue(cases(k, 1));
    assert_checkequal(block.getValue(), cases(k, 2));
end
controller.deleteObject(uid);
