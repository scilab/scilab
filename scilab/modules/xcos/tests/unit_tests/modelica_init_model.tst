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
// Modelica initialization GUI model on modelica_init.xml: a terminal directly
// under <elements>, and the Fix states / Fix derivatives actions.

// Any Xcos gateway loads the Xcos jars
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos"));
jimport java.io.File;
jimport org.scilab.modules.xcos.modelica.Modelica;
jimport org.scilab.modules.xcos.modelica.ModelicaController;

fixture = SCI + "/modules/xcos/tests/unit_tests/modelica_init.xml";
schema = xmlSchema(SCI + "/modules/xcos/etc/Modelica.xsd");

function checkvalid(f)
    doc = xmlRead(f);
    assert_checkequal(xmlValidate(doc, schema), []);
    xmlDelete(doc);
endfunction

// [fixed, weight] of each terminal of ids
function v = weights(f, ids)
    doc = xmlRead(f);
    v = [];
    for id = ids
        e = xmlXPath(doc, "//terminal[id=""" + id + """]");
        e = e(1);
        s = xmlXPath(e, "fixed");
        w = xmlXPath(e, "weight");
        if size(w, "*") == 0 then
            v = [v; s(1).attributes.value, ""];
        else
            v = [v; s(1).attributes.value, w(1).attributes.value];
        end
    end
    xmlDelete(doc);
endfunction

states = ["A.x" "A.T.[1]" "A.T.[2]"];
derivatives = ["A.__der_x" "A.__der_T.[1]" "A.__der_T.[2]" "A.__der_y"];

// ----------------------------------------------------------------------------
// A terminal directly under <elements>
// ----------------------------------------------------------------------------
checkvalid(fixture);

m = Modelica.getInstance();
model = m.load(File.new(fixture));
elements = model.getElements();
children = elements.getStructOrTerminal();
assert_checkequal(children.size(), int32(2));

w = TMPDIR + "/modelica_init.xml";
m.save(model, File.new(w));
checkvalid(w);
doc = xmlRead(w);
assert_checkequal(size(xmlXPath(doc, "/model/elements/terminal[id=""p""]"), "*"), 1);
xmlDelete(doc);

// ----------------------------------------------------------------------------
// Fix states, then Fix derivatives: only the terminals of weight 1 are fixed,
// a state may sit in another subtree than its derivative, or be missing
// ----------------------------------------------------------------------------
c = ModelicaController.new(m.load(File.new(fixture)));

c.fixWeight(0, 1);
m.save(c.getRoot(), File.new(w));
checkvalid(w);
assert_checkequal(weights(w, states), repmat(["true" "1.0"], 3, 1));
assert_checkequal(weights(w, derivatives), repmat(["false" "0.0"], 4, 1));
assert_checkequal(weights(w, "p"), ["true" ""]);

c.fixWeight(1, 0);
m.save(c.getRoot(), File.new(w));
assert_checkequal(weights(w, states), repmat(["false" "0.0"], 3, 1));
assert_checkequal(weights(w, derivatives), repmat(["true" "1.0"], 4, 1));
assert_checkequal(weights(w, "p"), ["true" ""]);

xmlDelete(schema);
