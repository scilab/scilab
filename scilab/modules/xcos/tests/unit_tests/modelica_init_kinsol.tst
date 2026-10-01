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
// Modelica initialization GUI on the Chaos_Modelica demo (Duffing oscillator),
// without the GUI: after Fix derivatives, x is a Kinsol unknown solving
// x - x^3 + gamma = 0, from a guess that is not a root.

ilib_verbose(0);
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/ModelicaBlocks/Chaos_Modelica.zcos"));
jimport java.io.File;
jimport org.scilab.modules.xcos.modelica.Modelica;
jimport org.scilab.modules.xcos.modelica.ModelicaController;

// The GUI window itself is not opened
prot = funcprot(0);
function xcosConfigureXmlFile(init, relations)
endfunction
funcprot(prot);

xcosConfigureModelica(scs_m);
name = stripblanks(scs_m.props.title(1)) + "_imf";
f = TMPDIR + "/" + name + "_init.xml";

function v = initial(f, id)
    doc = xmlRead(f);
    e = xmlXPath(doc, "//terminal[id=""" + id + """]/initial_value");
    v = strtod(e(1).attributes.value);
    xmlDelete(doc);
endfunction

// Guess typed in the table
doc = xmlRead(f);
e = xmlXPath(doc, "//terminal[id=""chaos1.x""]/initial_value");
e = e(1);
e.attributes.value = "2";
xmlWrite(doc, f);
xmlDelete(doc);

m = Modelica.getInstance();
c = ModelicaController.new(m.load(File.new(f)));
c.fixWeight(1, 0);
m.save(c.getRoot(), File.new(f));

assert_checktrue(compile_init_modelica(name, "0", "0"));
global icpr;
nx = size(icpr.state.x, "r") / 2;
assert_checkequal(nx, 1);
assert_checktrue(Compute_cic("Kinsol", string(nx)));

x = initial(f, "chaos1.x");
gamma = initial(f, "chaos1.gamma");
assert_checkfalse(x == 2);
assert_checkalmostequal(x - x^3 + gamma, 0, [], 1e-6);
