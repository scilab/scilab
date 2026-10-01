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
// Modelica initialization GUI files are UTF-8: a non-ASCII comment survives
// load / save cycles.

// Any Xcos gateway loads the Xcos jars
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos"));
jimport java.io.File;
jimport org.scilab.modules.xcos.modelica.Modelica;

comment = "Température moyenne (Ω)";
f = TMPDIR + "/modelica_init_encoding.xml";
mputl(["<model>"
"<name>encoding</name>"
"<elements>"
"<struct>"
"<name>A</name>"
"<subnodes>"
"<terminal>"
"<name>T</name>"
"<kind>variable</kind>"
"<id>A.T</id>"
"<fixed value=""""/>"
"<initial_value value=""""/>"
"<nominal_value value=""""/>"
"<comment value=""" + comment + """/>"
"</terminal>"
"</subnodes>"
"</struct>"
"</elements>"
"<equations/>"
"<when_clauses/>"
"</model>"], f);

m = Modelica.getInstance();
for k = 1:2
    m.save(m.load(File.new(f)), File.new(f));
    doc = xmlRead(f);
    e = xmlXPath(doc, "//terminal[id=""A.T""]/comment");
    assert_checkequal(e(1).attributes.value, comment);
    xmlDelete(doc);
end
