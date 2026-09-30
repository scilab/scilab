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
// Modelica initialization GUI on the RLC_Modelica demo, without the GUI:
// the Solve button steps of SolveAction after Fix states.

ilib_verbose(0);
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos"));
jimport java.io.File;
jimport org.scilab.modules.xcos.modelica.Modelica;
jimport org.scilab.modules.xcos.modelica.ModelicaController;

// Opening the GUI: xcosConfigureModelica
modelica_libs = getModelicaPath();
needcompile = 4;
%Modelica_Init = %t;
[bllst, connectmat, clkconnect, cor, corinv, ok] = c_pass1(scs_m);
%Modelica_Init = %f;
assert_checktrue(ok);
name = stripblanks(scs_m.props.title(1)) + "_imf";
f = TMPDIR + "/" + name + "_init.xml";

// Values typed in the table
function set_initial(f, values)
    doc = xmlRead(f);
    for k = 1:size(values, 1)
        e = xmlXPath(doc, "//terminal[id=""" + values(k, 1) + """]/initial_value");
        e = e(1);
        e.attributes.value = values(k, 2);
    end
    xmlWrite(doc, f);
    xmlDelete(doc);
endfunction
set_initial(f, ["Capacitor1.v" "1"; "Inductor1.i" "0"]);

m = Modelica.getInstance();
c = ModelicaController.new(m.load(File.new(f)));
c.fixWeight(0, 1);
assert_checktrue(c.isSquare());

// Solve
m.save(c.getRoot(), File.new(f));
assert_checktrue(compile_init_modelica(name, "0", "0"));
global icpr;
assert_checktrue(Compute_cic("Kinsol", string(size(icpr.state.x, "r") / 2)));

// The computed values are written back and the GUI reloads them
m.load(File.new(f));
doc = xmlRead(f);
assert_checkequal(size(xmlXPath(doc, "//terminal/initial_value[@value=""""]"), "*"), 0);
e = xmlXPath(doc, "//terminal[id=""Capacitor1.v""]/initial_value");
assert_checkequal(strtod(e(1).attributes.value), 1);
// Zero unless computed from Capacitor1.v
e = xmlXPath(doc, "//terminal[id=""Inductor1.__der_i""]/initial_value");
assert_checktrue(strtod(e(1).attributes.value) <> 0);
xmlDelete(doc);
