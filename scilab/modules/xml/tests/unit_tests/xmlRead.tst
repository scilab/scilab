// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 201 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================



xmlFile=xmlRead(SCI+"/etc/modules.xml");
content=xmlDump(xmlFile);
assert_checktrue(length(content)>0);
assert_checkequal(size(content),[1,1]);
xmlClose(xmlFile)
// Close the file a second time
assert_checkerror("xmlClose(xmlFile)","xmlClose: XML document does not exist");


xmlFile=xmlRead(SCI+"/etc/modules.xml");
xmlFile2=xmlRead(SCI+"/etc/classpath.xml");
content=xmlDump(xmlFile);
content2=xmlDump(xmlFile2);
//assert_checkequal(content, content2);
assert_checktrue(length(content)>0);
assert_checkequal(size(content),[1,1]);
assert_checktrue(length(content2)>0);
assert_checkequal(size(content2),[1,1]);
xmlClose(xmlFile)
xmlClose(xmlFile2)
// Close the file a second time
assert_checkerror("xmlClose(xmlFile)","xmlClose: XML document does not exist");

