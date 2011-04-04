// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
//
// <-- Short Description -->
// Check that Xcos never use Saxon (before or after building doc).

xcos(SCI + "/modules/xcos/demos/Simple_Demo.xcos");
closeXcos();

// load the saxon jar using a documentation Java call
execstr("buildDoc(""pdf"",""non-existing-file"",TMPDIR,TMPDIR);", "errcatch");

xcos(SCI + "/modules/xcos/demos/Simple_Demo.xcos");
closeXcos();

