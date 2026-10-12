// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
// <-- NO CHECK REF -->
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Short Description -->
// xcos(file) waits for the loading and reports its errors to the caller
//

demo = SCI + "/modules/xcos/demos/Simple_Demo.zcos";
xcos(demo);

// already opened: the diagram is reused
xcos(demo);

broken = TMPDIR + "/broken.xcos";
mputl(["<?xml version=""1.0"" encoding=""UTF-8""?>"; "<XcosDiagram>"], broken);
ierr = execstr("xcos(broken)", "errcatch");
assert_checktrue(ierr <> 0);

closeXcos();
