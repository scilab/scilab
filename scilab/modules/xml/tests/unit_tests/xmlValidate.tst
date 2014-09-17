// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlRead("SCI/modules/xml/tests/unit_tests/library.xml");
dtd = xmlDTD("SCI/modules/xml/tests/unit_tests/library.dtd");
schema = xmlSchema("SCI/modules/xml/tests/unit_tests/library.xsd");
rng = xmlRelaxNG("SCI/modules/xml/tests/unit_tests/library.rng");
assert_checkequal(doc.root.children.size,2);
xmlValidate(doc, dtd);

xmlValidate(doc, rng);

xmlValidate(doc, schema);

xmlValidate("SCI/modules/xml/tests/unit_tests/library.xml");
//xmlValidate("SCI/modules/xml/tests/unit_tests/invalid_library.xml")

xmlValidate("SCI/modules/xml/tests/unit_tests/library.xml", schema);
//xmlValidate("SCI/modules/xml/tests/unit_tests/invalid_library.xml", rng)

// Commented because I don't know how to check that now
//doc.root.children(3) = "<a>error</a>"

xmlValidate(doc, dtd);
xmlValidate(doc, rng);
xmlValidate(doc, schema);


xmlDelete(doc, dtd, schema, rng);

// Used to crash
assert_checktrue(size(xmlValidate("SCI/DO_NOT_EXIST.XML"))>0);
